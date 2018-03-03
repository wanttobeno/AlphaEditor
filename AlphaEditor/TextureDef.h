#ifndef TEXTUREDEF_H__
#define TEXTUREDEF_H__


enum TEXT_STYLE
{
	TEXT_NONE =0,
	TEXT_LINE_FRAME,	// 线框
	TEXT_ARROW,			// 箭头
	TEXT_ELIPSE,		// 椭圆
	TEXT_RECT,			// 矩形
	TEXT_ROUND,			// 圆形
	TEXT_CLOUDS,		// 云朵
	TEXT_BLOW,			// 爆炸
};


enum TEXT_STATE
{
	TEXT_STATE_NORMAL=0,		// 文字，图片
	TEXT_STATE_FOCUS,			// 编辑状态
	TEXT_STATE_KILLFOCUS,
	TEXT_STATE_MOUSEENTER,		// 鼠标移入
	TEXT_STATE_MOUSELEAVE,    // 鼠标移除
	TEXT_STATE_LBUTTONDOWN,   // 鼠标左键按下
	TEXT_STATE_KEYDOWN,			// 鼠标案件按下
};




#endif //TEXTUREDEF_H__
