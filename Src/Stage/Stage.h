/**
* @file Stage.h
*/

#ifndef STAGE_H_INCLUDE
#define STAGE_H_INCLUDE

#include <vector>
#include <string>

class Stage
{
public:
	Stage(std::string filename);
	virtual ~Stage();

	void Create();

private:
	Stage(const Stage&) = delete;
	Stage& operator=(const Stage&) = delete;

	// �}�b�v�̃T�C�Y
	int size_x, size_y;
	// �}�b�v�̃f�[�^
	std::vector<std::vector<int>> mapdata;

};

#endif // STAGE_H_INCLUDE