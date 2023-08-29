#include "Vector.h"
#include "RenderContext.h"
#include "Font.h"


void Font::Begin(RenderContext& rc)
{
	auto& fontEngine = g_graphicsEngine->GetFontEngine();
	fontEngine.BeginDraw(rc);
}
void Font::End(RenderContext& rc)
{
	auto& fontEngine = g_graphicsEngine->GetFontEngine();
	fontEngine.EndDraw(rc);
}