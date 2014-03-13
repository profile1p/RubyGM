#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: ����Direct2D ���϶���Ļ������(��һ���ǻ�����)

class GeometryCahce{
public:
	// �½�Ruby ���ζ���
	static VALUE NewGeometry(int argc, VALUE* argv, VALUE self);
	// �ͷŶ���
	static VALUE Dispose(VALUE self);
	// ��ʼ����
	static VALUE DeginPaint(VALUE self);
	// ��������
	static VALUE EndPaint(VALUE self);
public:
	// �ƶ����Ƶ�

public:
	// D2D ����
	typedef ::ID2D1Factory1 D2DFactory;
	// ·������
	typedef	::ID2D1PathGeometry1 PathGeometry;
	// Geometry Sink
	typedef ::ID2D1GeometrySink GeometrySink;
	// �ı����϶������
	struct FontGeometryParameter
	{
		// ����
		IDWriteFontFace*			pFontFace;
		// �����߼���С
		FLOAT						emSize;
		// �ı�����
		WCHAR*						pString;
		// �ı�����
		UINT						uStringLength;
		// ���Ϊ TRUE�������ε������ػ������С����Ϊ FALSE�������ε�������ֱ�ڻ�������
		BOOL						isSideways;
		// ���ε��Ӿ�˳������˲���Ϊ FALSE�������δ������Ҳ��������Ϊ TRUE���򲽽�����Ϊ��������
		BOOL						isRightToLeft;
		// ���
		PathGeometry*				pPathGeometry;
	};
public:
	// �����ı�����
	static HRESULT CreateFontGeometry(FontGeometryParameter* pParameter);
public:
	// D2D ����
	static D2DFactory**				s_ppD2DFactory;
private:
	// �������

};