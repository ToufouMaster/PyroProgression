#pragma once
#include "../CWSDK/cwsdk.h"
#include "utility.h"

void Setup_ArtifactStatsOverwrite() {
	WriteByte(CWOffset(0x66883), 0x90);
	WriteByte(CWOffset(0x66884), 0x90);
	WriteByte(CWOffset(0x66885), 0x90);
}