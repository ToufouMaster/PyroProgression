#pragma once
#include "../CWSDK/cwsdk.h"
#include "utility.h"

extern "C" void cube__Game__LoadCharacter(cube::Game *game, unsigned int *character_slot, cube::Creature* creature)
{

}

GETTER_VAR(void*, ASM_cube__Game__LoadCharacter_jmpback);
__attribute__((naked)) void ASM_cube__Game__LoadCharacter() {
	asm(".intel_syntax \n"
		PUSH_ALL

		// put result on stack
		"mov eax, edx \n"
		"push rax \n"
		"lea rdx, [rsp] \n"

		PREPARE_STACK
		"call cube__Game__LoadCharacter \n"
		RESTORE_STACK

		// get result
		"pop rax \n"
		"mov edx, eax \n"

		POP_ALL

		// Old code
		"lea rbp, [rsp-0x130] \n"
		"sub rsp, 0x230 \n"
		DEREF_JMP(ASM_cube__Game__LoadCharacter_jmpback)
	);
}

void Setup_cube__Game__LoadCharacter() {
	WriteFarJMP(CWOffset(0x9BB28), (void*)&ASM_cube__Game__LoadCharacter);
	ASM_cube__Game__LoadCharacter_jmpback = CWOffset(0x9BB37);
}