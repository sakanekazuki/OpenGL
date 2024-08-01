/**
* @file Audio.h
*/

#ifndef AUDIO_H_INCLDUE
#define AUDIO_H_INCLDUE

#define NOMINMAX
#undef APIENTRY

#include <cri_adx2le.h>
#include <vector>

/**
* 音声制御
*/
class Audio
{
public:
	static bool Initialize(const char* acfPath, const char* dspBusName);
	static void Finalize();
	static Audio& Get();

	// ACB,AWBファイル管理
	bool Load(size_t index, const char* acbPath, const char* acwPath);
	void Unload(size_t index);

	void Update();

	void Play(size_t playerId, int cueId);
	void Stop(size_t playerId);

	bool IsStop(size_t playerId);

private:
	Audio() = default;
	~Audio() = default;
	Audio(const Audio&) = default;
	Audio& operator=(const Audio&) = default;

	CriAtomExVoicePoolHn voicePool = nullptr;
	CriAtomDbasId dbasId = CRIATOMDBAS_ILLEGAL_ID;
	std::vector<CriAtomExAcbHn> acbList;
	std::vector<CriAtomExPlayerHn> players;
	// キューIDからACBハンドルを取得
	std::vector<CriAtomExAcbHn> cueIdToAcbMap;


};

#endif // AUDIO_H_INCLDUE