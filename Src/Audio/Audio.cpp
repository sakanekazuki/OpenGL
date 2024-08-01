/**
* @file Audio.cpp
*/

#include "Audio.h"

#include <algorithm>
#include <iostream>

namespace
{
	Audio* audio = nullptr;

	/**
	* オーディオ用エラーコールバック
	* 
	* @param errid  エラーの種類を示すID
	* @param p1     erridの補足情報その1
	* @param p2     erridの補足情報その2
	* @param parray (未使用)
	*/
	void AudioErrorCallback(
		const CriChar8* errid,
		CriUint32 p1,
		CriUint32 p2,
		CriUint32* parray)
	{
		const CriChar8* err = criErr_ConvertIdToMessage(errid, p1, p2);
		std::cerr << err << "\n";
	}

	/**
	* オーディオ用メモリ確保
	* 
	* @param obj   登録時に指定したユーザー引数
	* @param seize 確保するバイト数
	* 
	* @retval 確保したメモリアドレス
	*/
	void* AudioAllocate(void* obj, CriUint32 size)
	{
		return operator new(size);
	}

	/**
	* オーディオ用メモリ解放
	* 
	* @param obj 登録時に指定したユーザー引数
	* @param ptr 解放するメモリのアドレス
	*/
	void AudioDeallocate(void* obj, void* ptr)
	{
		operator delete(obj);
	}
}

/**
* 音声システムの初期化
* 
* @param acfPath    全体設定を補間するACFファイルのパス
* @param dspBusName 音声システムで使用するDSPパス名
* 
* @retval true  初期化に成功
* @retval false 初期化に失敗
*/
bool Audio::Initialize(
	const char* acfPath,
	const char* dspBusName)
{
	// オーディオが存在しない場合作成
	if (!audio)
	{
		audio = new Audio;

		// エラーコールバック関数を登録
		criErr_SetCallback(AudioErrorCallback);

		// メモリ管理関数を登録
		criAtomEx_SetUserAllocator(AudioAllocate, AudioDeallocate, nullptr);

		// 初期化パラメータを設定
		CriAtomExConfig_WASAPI libConfig;
		criAtomEx_SetDefaultConfig_WASAPI(&libConfig);

		// 
		CriFsConfig fsConfig;
		criFs_SetDefaultConfig(&fsConfig);
		// num_voicesより大きい値を設定する
		fsConfig.num_loaders = 40;
		libConfig.atom_ex.fs_config = &fsConfig;

		// 再生制御可能な音声の最大数. 実際の発音数はボイスプールのnum_voicesで指定する
		libConfig.atom_ex.max_virtual_voices = 64;

		// OpenGL用に右手座標系を指定
		libConfig.atom_ex.coordinate_system = CRIATOMEX_COORDINATE_SYSTEM_RIGHT_HANDED;

		// Atomライブラリを初期化
		criAtomEx_Initialize_WASAPI(&libConfig, nullptr, 0);

		// ストリーミング用バッファを作成
		audio->dbasId = criAtomDbas_Create(nullptr, nullptr, 0);

		// ACFファイルを読み込む
		if (criAtomEx_RegisterAcfFile(nullptr, acfPath, nullptr, 0) == CRI_FALSE)
		{
			std::cerr << "[エラー]" << __func__ << " : " << acfPath << "の読み込みに失敗\n";
			Finalize();
			return false;
		}

		// DSPパスを割り当てる
		criAtomEx_AttachDspBusSetting(dspBusName, nullptr, 0);

		// ボイスプールを設定
		CriAtomExStandardVoicePoolConfig svpConfig;
		criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&svpConfig);
		// 同時発音数
		svpConfig.num_voices = libConfig.atom_ex.max_virtual_voices / 2;
		// ストリーミング再生を有効化
		svpConfig.player_config.streaming_flag = CRI_TRUE;
		// 最大サンプリングレート、ピッチ変更を考慮してCD音質の2倍を設定
		svpConfig.player_config.max_sampling_rate =
			48000 * 2;
		audio->voicePool = criAtomExVoicePool_AllocateStandardVoicePool(
			&svpConfig, nullptr, 0);

		// 再生制御用プレイヤーを作成
		audio->players.resize(8);
		for (auto& e : audio->players)
		{
			e = criAtomExPlayer_Create(nullptr, nullptr, 0);
		}
		// acb読み込み配列を確保
		audio->acbList.resize(16);

		// キューIDとacbファイルの対応表を確保
		audio->cueIdToAcbMap.resize(4096);

		std::cerr << "[情報]" << __func__ << ": オーディオ機能を初期化\n";
	}
	// 初期化成功
	return true;
}

/**
* 音声システムを破棄
*/
void Audio::Finalize()
{
	// 既に破棄されている場合はなにもしない
	if (!audio)
	{
		return;
	}

	// 全てのプレイヤーを破棄

	for (auto& e : audio->players)
	{
		if (e)
		{
			criAtomExPlayer_Destroy(e);
			e = nullptr;
		}
	}

	// 全てのACBファイル破棄
	for (auto& e : audio->acbList)
	{
		if (e)
		{
			criAtomExAcb_Release(e);
			e = nullptr;
		}
	}

	// キューIDとACBの対応表を初期化
	std::fill(audio->cueIdToAcbMap.begin(), audio->cueIdToAcbMap.end(), nullptr);

	// ボイスプールを破棄
	if (audio->voicePool)
	{
		criAtomExVoicePool_Free(audio->voicePool);
		audio->voicePool = nullptr;
	}

	// ACFファイルの登録を解除
	criAtomEx_UnregisterAcf();

	// DCASを破棄
	if (audio->dbasId != CRIATOMDBAS_ILLEGAL_ID)
	{
		criAtomDbas_Destroy(audio->dbasId);
		audio->dbasId = CRIATOMDBAS_ILLEGAL_ID;
	}

	// ADXLEを終了
	criAtom_Finalize_WASAPI();
	delete audio;
	audio = nullptr;

	std::cerr << "[情報]" << __func__ << ": オーディオ機能を破棄\n";
}

/**
* 音声制御クラスの取得
* 
* @param 音声制御クラスのインスタンスの参照
*/
Audio& Audio::Get()
{
	return *audio;
}

/**
* 音声ファイルを読み込む
* 
* @param index   読み込み先のACB配列の番号
* @param acbPath ACBファイルのパス名
* @parma awbPath AWBファイルのパス名
* 
* @retval true  読み込み成功
* @retval false 読み込み失敗
*/
bool Audio::Load(size_t index, const char* acbPath, const char* awbPath)
{
	if (index >= acbList.size())
	{
		std::cerr << "[エラー]" << __func__ << " : " <<
			acbPath << "のインデックスが大きすぎます\n";
		return false;
	}

	// 念のため読み込む
	acbList[index] = criAtomExAcb_LoadAcbFile(
		nullptr, acbPath, nullptr, awbPath, nullptr, 0);
	if (!acbList[index])
	{
		std::cerr << "[エラー]" << __func__ << " : " << "の読み込みに失敗\n";
		return false;
	}

	// キューIDとACBの対応表を更新
	const CriSint32 numCues = criAtomExAcb_GetNumCues(acbList[index]);
	for (int i = 0; i < numCues; ++i)
	{
		// キュー情報の取得
		CriAtomExCueInfo cueInfo;
		if (!criAtomExAcb_GetCueInfoByIndex(acbList[index], i, &cueInfo))
		{
			std::cerr << "[警告]" << __func__ << " : " << acbPath << "の" <<
				i << "番目のキュー情報を取得できません\n";
			continue;
		}
		// 対応表よりキューIDが大きい場合は対応表のサイズを拡張
		if (cueIdToAcbMap.size() <= static_cast<int>(cueInfo.id))
		{
			cueIdToAcbMap.resize(cueInfo.id + 1);
		}
		// キューIDとACBを対応付ける
		cueIdToAcbMap[cueInfo.id] = acbList[index];
	}
	return true;
}

/**
* オーディオファイルを破棄する
* 
* @param index 破棄するACB配列の番号
*/
void Audio::Unload(size_t index)
{
	if (index >= acbList.size())
	{
		std::cerr << "[エラー]" << __func__ << " : " <<
			"インデックスが大きすぎます\n";
		return;
	}

	if (acbList[index])
	{
		// 対応表から破棄予定のACBハンドルを消去
		std::replace(cueIdToAcbMap.begin(), cueIdToAcbMap.end(),
			acbList[index], static_cast<CriAtomExAcbHn>(nullptr));

		// ACBハンドルを破棄
		criAtomExAcb_Release(acbList[index]);
		acbList[index] = nullptr;
	}
}

/**
* 音声システムの状態を更新
*/
void Audio::Update()
{
	// Atomライブラリの状態を更新
	criAtomEx_ExecuteMain();
}

/**
* 音声を再生
* 
* @param playerId 再生に使用するプレイヤー番号
* @param cueId    再生するキューID
*/
void Audio::Play(size_t playerId, int cueId)
{
	// プレイヤー番号がプレイヤー数以上の場合はなにもしない
	if (playerId >= players.size())
	{
		return;
	}
	//対応表がnullptrの場合は何もしない
	if (!cueIdToAcbMap[cueId])
	{
		return;
	}

	// プレイヤーにキューをセット
	criAtomExPlayer_SetCueId(players[playerId], cueIdToAcbMap[cueId], cueId);

	// セットしたキューを再生
	criAtomExPlayer_Start(players[playerId]);
}

/**
* 音声を停止
* 
* @param playerId 再生を停止するプレイヤー番号
*/
void Audio::Stop(size_t playerId)
{
	// プレイヤー番号がプレイヤー数以上の場合は何もしない
	if (playerId >= players.size())
	{
		return;
	}

	// 再生を停止
	criAtomExPlayer_Stop(players[playerId]);
}

/**
* 音声が止まっているかどうか調べる
* 
* @param playerId 調べるプレイヤー番号
*/
bool Audio::IsStop(size_t playerId)
{
	// 音声の状態の取得
	CriAtomExPlayerStatus status = criAtomExPlayer_GetStatus(players[playerId]);

	return status == CriAtomExPlayerStatus::CRIATOMEXPLAYER_STATUS_PLAYEND ||
		status == CriAtomExPlayerStatus::CRIATOMEXPLAYER_STATUS_STOP;
}