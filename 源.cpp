/****************************************************
* �������ƣ�ʵ��ˮ������ʾ							*
* ���뻷����Visual C++ 6.0��EasyX 20130114(beta)	*
* �������ߣ��� (QQ: 415051674) 					*
* �����㷨���ο������㷨							*
* ����޸ģ�2013/3/20								*
****************************************************/
/* ����ѧȥ����վ�ռ� http://www.zixue7.com */

#include <graphics.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>

//�����ͼƬ�ĳߴ粻һ���ᵼ��ͼƬ���س���
#define PIC_HEIGHT 705 //ͼƬ�ĸ߶�
#define PIC_WIDTH  960 //ͼƬ�Ŀ��
#define PICTURE  "bg2.JPG"//ͼƬ������

void FrameFun();									// ֡�߼�����������ÿһ֡���߼�
void RenderFun();									// ֡��Ⱦ���������ÿһ֡����ʾ�豸

IMAGE src_img;										// ԭλͼ		
IMAGE dest_img(PIC_WIDTH, PIC_HEIGHT);				// �������ʾ��λͼ
DWORD* img_ptr1;									// ԭͼƬƬ�ڴ�ָ��
DWORD* img_ptr2;									// �������ʾ��λͼ�ڴ�ָ��


// �������� buf Ϊÿһ����Ĳ�����ǰ��Ϊ��ǰ����������Ϊ��һ��ʱ�̵Ĳ�����
short* buf = new short[PIC_HEIGHT * PIC_WIDTH + PIC_WIDTH];
short* buf2 = new short[PIC_HEIGHT * PIC_WIDTH + PIC_WIDTH];
//short *buf = new short[PIC_HEIGHT*PIC_WIDTH];
//short *buf2 = new short[PIC_HEIGHT*PIC_WIDTH];


int main()
{
	// ��ʼ���豸������ͼƬ
	initgraph(PIC_WIDTH, PIC_HEIGHT);
	SetWindowText(GetHWnd(), "ˮ������Ч");
	loadimage(&src_img, PICTURE);		// ����ͼƬ
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	//settextstyle(25, 0, "Rock");

	// ����ڴ�ָ��
	img_ptr1 = GetImageBuffer(&src_img);
	img_ptr2 = GetImageBuffer(&dest_img);

	// ��ʼ����������
	memset(buf, 0, (PIC_HEIGHT * PIC_WIDTH + PIC_WIDTH) * sizeof(short));
	memset(buf2, 0, (PIC_HEIGHT * PIC_WIDTH + PIC_WIDTH) * sizeof(short));

	// Let's Go!
	BeginBatchDraw();		// ˫���壬����ʱ��Ҫ
	while (true)
	{
		FrameFun();
		RenderFun();
		FlushBatchDraw();
		Sleep(3);
	}
	EndBatchDraw();
}

// �������һ��ʱ�����е�Ĳ���
void nextFrame()
{
	for (int i = PIC_WIDTH; i < PIC_HEIGHT * (PIC_WIDTH - 1); i++)
	{
		// ��ʽ��X0'= (X1+X2+X3+X4) / 2 - X0
		buf2[i] = ((buf[i - PIC_WIDTH] + buf[i + PIC_WIDTH] + buf[i - 1] + buf[i + 1]) >> 1) - buf2[i];

		// ����˥��,ƫ�Ƶķ�Χ
		buf2[i] -= buf2[i] >> 12;
	}

	short* ptmp = buf;
	buf = buf2;
	buf2 = ptmp;
}


void RenderRipple3() {
	int xoff, yoff;
	int k = PIC_WIDTH;
	for (int i = 1; i < PIC_HEIGHT - 1; i++) {
		for (int j = 0; j < PIC_WIDTH; j++) {
			xoff = buf[k - 1] - buf[k + 1];
			yoff = buf[k - PIC_WIDTH] - buf[k + PIC_WIDTH];

			if (i + yoff < 0) {
				k++;
				continue;
			}
			if (i + yoff > PIC_HEIGHT) {
				k++;
				continue;
			}
			if (j + xoff < 0) {
				k++;
				continue;
			}
			if (j + xoff > PIC_WIDTH) {
				k++;
				continue;
			}

			int index = (i + yoff) * PIC_WIDTH + j + xoff;
			img_ptr2[k++] = img_ptr1[index];

		}
	}
}

// ����ǰʱ�̲���Ӱ��֮���λͼ�������� dest_img ��
void RenderRipple2()
{
	int i = 0;
	//   ---> x
	//   |
	// y V  
	for (int y = 0; y < PIC_HEIGHT; y++)
	{
		for (int x = 0; x < PIC_WIDTH; x++)
		{
			short data = 1024 - buf[i];

			// ƫ��
			int a = ((x - PIC_WIDTH / 2) * data / 1024) + PIC_WIDTH / 2;
			int b = ((y - PIC_HEIGHT / 2) * data / 1024) + PIC_HEIGHT / 2;

			// �߽紦��
			if (a >= PIC_WIDTH)		a = PIC_WIDTH - 1;
			if (a < 0)				a = 0;
			if (b >= PIC_HEIGHT)	b = PIC_HEIGHT - 1;
			if (b < 0)				b = 0;

			// ����ƫ�� 
			img_ptr2[i] = img_ptr1[a + (b * PIC_WIDTH)];

			i++;
		}
	}
}

// ����ǰʱ�̲���Ӱ��֮���λͼ�������� dest_img ��
void RenderRipple()
{
	int i = 0;
	for (int y = 0; y < PIC_HEIGHT; y++)
	{
		for (int x = 0; x < PIC_WIDTH; x++)
		{
			short data = 1024 - buf[i];

			// ƫ��
			int a = ((x - PIC_WIDTH / 2) * data / 1024) + PIC_WIDTH / 2;
			int b = ((y - PIC_HEIGHT / 2) * data / 1024) + PIC_HEIGHT / 2;

			// �߽紦��
			if (a >= PIC_WIDTH)		a = PIC_WIDTH - 1;
			if (a < 0)				a = 0;
			if (b >= PIC_HEIGHT)	b = PIC_HEIGHT - 1;
			if (b < 0)				b = 0;

			// ����ƫ�� 
			img_ptr2[i] = img_ptr1[a + (b * PIC_WIDTH)];

			i++;
		}
	}
}

// ���ģ��Ͷʯͷ
// ����˵����
// (x, y): �������
// stonesize: ��ʯͷ���Ĵ�С,(ʯͷ�İ뾶��
// stoneweight: Ͷ��ʯͷ��������
// Ps: �����������һ���������Խ�����£������������ʯͷ���Ĵ�С�͡�ʯͷ��������
void disturb(int x, int y, int stonesize, int stoneweight)
{
	// ͻ�Ʊ߽粻����
	if ((x >= PIC_WIDTH - stonesize) ||
		(x < stonesize) ||
		(y >= PIC_HEIGHT - stonesize) ||
		(y < stonesize))
		return;

	for (int posx = x - stonesize; posx < x + stonesize; posx++)
	{
		for (int posy = y - stonesize; posy < y + stonesize; posy++)
		{
			if ((posx - x) * (posx - x) + (posy - y) * (posy - y) < stonesize * stonesize)
			{
				buf[PIC_WIDTH * posy + posx] += stoneweight;
			}
		}
	}
}

// ����fps
float getFps()
{
#define FPS_COUNT 8
	static int i = 0;
	static int oldTime = GetTickCount();
	static float fps;

	if (i > FPS_COUNT)
	{
		i = 0;
		int newTime = GetTickCount();
		int elapsedTime = newTime - oldTime;
		fps = FPS_COUNT / (elapsedTime / 1000.0f);
		oldTime = newTime;
	}
	i++;
	return fps;
}

// ��Ⱦ
void RenderFun()
{
	//RenderRipple();
	RenderRipple2();

	putimage(0, 0, &dest_img);

	char s[15];
	sprintf_s(s, "%.1f", getFps());
	outtextxy(0, 0, s);
}

// �߼�
void FrameFun()
{
	// ���������
	if (MouseHit()){
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_MOUSEMOVE){
			disturb(msg.x, msg.y, 2, 256);
		}else if (msg.uMsg == WM_LBUTTONDOWN){
			disturb(msg.x, msg.y, 2, 2560);
		}
		FlushMouseMsgBuffer();
	}
	// ������һ֡�Ĳ���
	nextFrame();
}