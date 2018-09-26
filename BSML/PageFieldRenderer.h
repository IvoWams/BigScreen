using namespace std;

#ifndef PAGEFIELDRENDERER_H
#define PAGEFIELDRENDERER_H

#include <props/PageField.h>
#include <global/Color.h>

class PageFieldRenderer : public ParserListener {
	private:
		MainEngine*	engine;
		PageField*	pagefield;
		Font*		font;
		
//		Font*	font;
//		int		fontsize;
//		Color*	fontcolor;
		
	public:
		PageFieldRenderer(PageField*);		// someone listens to renderer?
		~PageFieldRenderer();

		void onOpeningTag(const char*);
		void onAttribute(const char*, const char*, const char*);
		void onAttribute(const char*, const char*, double);
		void onClosingTag(const char*);
		
		void onAnyText(const char*);
};

#endif