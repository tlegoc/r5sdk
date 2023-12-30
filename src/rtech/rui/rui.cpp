//=============================================================================//
//
// Purpose: rUI Utilities
//
//=============================================================================//

#include "core/stdafx.h"

#ifndef DEDICATED

#include "rui.h"
#include "tier1/cvar.h"
#include "windows/id3dx.h"

//-----------------------------------------------------------------------------
// Purpose: draw RUI frame
//-----------------------------------------------------------------------------
// 100% Sure ? from what i've seen this is called way after the UI itself is drawn.
// also is called per object drawn.
bool __fastcall Rui_Draw(__int64* a1, __m128* a2, const __m128i* a3, __int64 a4, __m128* a5)
{
	// A1 is probably time
	// A2 is probably a vec4 cause the last 64 bits are width-height. no idea what the first two floats are

	if (!rui_drawEnable->GetBool())
		return false;

	return v_Rui_Draw(a1, a2, a3, a4, a5);
}

void V_Rui::Detour(const bool bAttach) const
{
	DetourSetup(&v_Rui_Draw, &Rui_Draw, bAttach);
}

#endif // !DEDICATED