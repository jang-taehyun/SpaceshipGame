#pragma once

// -------- typedef -------- //

// shader ���� ����
typedef struct _shader_file_info_
{
	tstring vsFileName = { 0, };
	tstring psFileName = { 0, };
	tstring vsEntryPoint = { 0, };
	tstring psEntryPoint = { 0, };
} ShaderFileInfo;

// sound ���� ����
typedef struct _info_
{
	tstring filename = { 0, };
} SoundInfo;

// ------------------------ //

// data //

const SoundInfo s_info = { _T("test") };
const ShaderFileInfo shaderInfo = { _T("test"), _T("test"), _T("test"), _T("test"), };

// ------------------------ //