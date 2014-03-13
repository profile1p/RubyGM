#include "stdafx.h"
#include "include.h"


#define ppCacheRT BitmapCache::s_ppRenderTarget


#ifdef USING_CPlusPlus_MODE
// 清除笔刷缓存
void BrushCache::Clear(){
	for (BrushCache::Map::iterator itr = s_mapBrushCache.begin(); itr != s_mapBrushCache.end(); ++itr){
		SafeRelease(itr->second);
	}
	s_mapBrushCache.clear();
}


// 重建笔刷缓存
void BrushCache::Recreate(){
	for (BrushCache::Map::iterator itr = s_mapBrushCache.begin(); itr != s_mapBrushCache.end(); ++itr){
		SafeRelease(itr->second);

	}
}

// 获取对应笔刷
ID2D1Brush*	BrushCache::GetBrush(const GameBrush& brush_info){
	BrushCache::Map::iterator itr = s_mapBrushCache.find(brush_info);
	if (itr != s_mapBrushCache.end())
		return itr->second;
	return CreateBrush(brush_info);
}

// 创建笔刷
ID2D1Brush*	BrushCache::CreateBrush(const GameBrush& brush_info){
	ID2D1SolidColorBrush* sbrush;
	ID2D1GradientStopCollection * stops;
	ID2D1LinearGradientBrush * lbrush;
	ID2D1RadialGradientBrush * rbrush;;
#ifdef _DEBUG
	BrushCache::Map::iterator itr = s_mapBrushCache.find(brush_info);
	if (itr != s_mapBrushCache.end())
		check_pointer(ppCacheRT, "Brush Created!\n");
	check_pointer(ppCacheRT, "BitmapCache::s_ppRenderTarget -> 0\n");
	check_pointer(*ppCacheRT, "*BitmapCache::s_ppRenderTarget -> 0\n");
#endif
	if (brush_info.stop_count < 2){
		// 创建纯色笔刷
#ifdef _DEBUG
		if (FAILED((*ppCacheRT)->CreateSolidColorBrush(brush_info.gradient_stops->color, &sbrush)))
			_cwprintf(L"纯色笔刷创建失败\n");
#else
		(*ppCacheRT)->CreateSolidColorBrush(brush_info.gradient_stops->color, &sbrush);
#endif
		s_mapBrushCache[brush_info] = sbrush;
		return sbrush;
	}
	else{
		// 创建渐变停止点集合
		if (SUCCEEDED((*ppCacheRT)->CreateGradientStopCollection(brush_info.gradient_stops, brush_info.stop_count,
			D2D1_GAMMA_2_2,	D2D1_EXTEND_MODE_CLAMP, &stops))){
			switch (brush_info.type)
			{
			case GameBrush::BrushType::Linear:
				// 创建线性渐变笔刷
#ifdef _DEBUG
				if (FAILED((*ppCacheRT)->CreateLinearGradientBrush(brush_info.linear_properties, stops, &lbrush)))
					_cwprintf(L"创建线性渐变笔刷创建失败\n");
#else
				(*ppCacheRT)->CreateLinearGradientBrush(brush_info.linear_properties, stops, &lbrush);
#endif
				SafeRelease(stops);
				return lbrush;
			case GameBrush::BrushType::Radial:
			default:
				// 创建径向渐变笔刷
#ifdef _DEBUG
				if (FAILED((*ppCacheRT)->CreateRadialGradientBrush(brush_info.radial_properties, stops, &rbrush)))
					_cwprintf(L"创建径向渐变笔刷创建失败\n");
#else
				(*ppCacheRT)->CreateRadialGradientBrush(brush_info.radial_properties, stops, &rbrush);
#endif
				SafeRelease(stops);
				return rbrush;

			}
		}
#ifdef _DEBUG
		else
			_cwprintf(L"渐变停止点集合创建失败\n");
#endif
		SafeRelease(stops);
	}
	return NULL;
}

#else
// Ruby端 创建Brush
VALUE BrushCache::RB_CreateBrush(int argc, VALUE* argv, VALUE self){
	//
	// [0x66ccff, 1.0]
	// [[0x66ccff, 1.0, 0.0], []], type, [0.0 , 5.0 , 80.0, 60.0, XX, XX]
	if (argc <= 1 || argc == 3 || argc > 4){
		rb_raise(rb_eArgError, "wrong number of arguments\n参数数量出错");
		return Qfalse;
	}
	if (RBASIC(argv[1])->klass != rb_cArray){
		rb_raise(rb_eArgError, "INVALID arguments\n参数无效:");
		return Qfalse;
	}
	if (argc == 2){
		// 纯色笔刷
		GameBrushParameter parameter;
		parameter.this_length = sizeof GameBrushParameter;
		parameter.gradient_stops->color = D2D1::ColorF::ColorF(
			FIX2INT(rb_ary_entry(argv[1], 0)),
			RB_INT_FLOAT_TO_FLOAT(rb_ary_entry(argv[1], 1))
			);
		return SetBrush(FIX2INT(*argv), &parameter) ? Qtrue : Qfalse;
	}
	else{
		RArray* pArray1 = RARRAY(argv[1]);
		RArray* pArray2 = RARRAY(argv[3]);
		if (pArray2->basic.klass != rb_cArray || (pArray2->as.heap.len != 4 && pArray2->as.heap.len != 6)) {
			rb_raise(rb_eArgError, "INVALID arguments\n参数无效:");
			return Qfalse;
		}
		GameBrushParameter* pointer(nullptr);
		// 计算停止点数量
		UINT stop_count = pArray1->as.heap.len / 3;
		// 计算长度
		UINT length = sizeof(GameBrushParameter)+(stop_count - 1)*sizeof(D2D1_GRADIENT_STOP);
		// 申请空间
		if (pointer = (GameBrushParameter*)MPool.Alloc(length)){
			new(pointer)GameBrushParameter();
			pointer->this_length = length;
			pointer->stop_count = length;
			for (UINT i = 0; i < stop_count; ++i){
				pointer->gradient_stops[i].color = D2D1::ColorF::ColorF(
					FIX2INT(pArray1->as.heap.ptr[i * 3]),
					(FLOAT)RFLOAT(pArray1->as.heap.ptr[i * 3 + 1])->float_value
					);
				pointer->gradient_stops[i].position = (FLOAT)RFLOAT(pArray1->as.heap.ptr[i * 3 + 2])->float_value;
			}
			switch (pointer->brush_type = static_cast<BrushType>(FIX2INT(argv[2])))
			{
			case BrushType::Linear:
				pointer->linear_properties.startPoint.x = RB_INT_FLOAT_TO_FLOAT(pArray2->as.heap.ptr[0]);
				pointer->linear_properties.startPoint.y = RB_INT_FLOAT_TO_FLOAT(pArray2->as.heap.ptr[1]);
				pointer->linear_properties.endPoint.x = RB_INT_FLOAT_TO_FLOAT(pArray2->as.heap.ptr[2]);
				pointer->linear_properties.endPoint.y = RB_INT_FLOAT_TO_FLOAT(pArray2->as.heap.ptr[3]);
				break;
			case BrushType::Radial:
				pointer->radial_properties.center.x = RB_INT_FLOAT_TO_FLOAT(pArray2->as.heap.ptr[0]);
				pointer->radial_properties.center.y = RB_INT_FLOAT_TO_FLOAT(pArray2->as.heap.ptr[1]);
				pointer->radial_properties.gradientOriginOffset.x = RB_INT_FLOAT_TO_FLOAT(pArray2->as.heap.ptr[2]);
				pointer->radial_properties.gradientOriginOffset.y = RB_INT_FLOAT_TO_FLOAT(pArray2->as.heap.ptr[3]);
				pointer->radial_properties.radiusX = RB_INT_FLOAT_TO_FLOAT(pArray2->as.heap.ptr[4]);
				pointer->radial_properties.radiusX = RB_INT_FLOAT_TO_FLOAT(pArray2->as.heap.ptr[5]);
				break;
			default:
				MPool.Free(pointer);
				return Qfalse;
			}
			auto rcode = SetBrush(FIX2INT(*argv), pointer);
			MPool.Free(pointer);
			return rcode ? Qtrue : Qfalse;
		}
	}
	return Qfalse;
}

// 初始化
void	BrushCache::Init(){
	// 载入系统笔刷
	std::ifstream ifs("Data/system.brs", std::ios::binary);
	UINT32 data, length;
	BrushCache::GameBrushParameter* pParameter(nullptr);
	if (ifs.good()){
		// 读取首数据
		ifs.read((char*)&data, sizeof data);
		for (UINT i = 0; i < (data & 0xFFFF); ++i){
			// 读取长度
			ifs.read((char*)&length, sizeof length);
			if (length){
				if (pParameter = (GameBrushParameter*)MPool.Alloc(length)){
					pParameter->this_length = length;
					ifs.read((char*)&(pParameter->brush_type), length - sizeof(length));
					SetBrush(i, pParameter);
					MPool.Free(pParameter);
				}
			}
		}
		ifs.close();
	}
	// C++自定义
}

// 清除笔刷缓存
void BrushCache::Clear(){
	for (auto& i : s_aryBrushCache){
		SafeRelease(i);
	}
}


// 重建笔刷缓存
void BrushCache::Recreate(){
	BrushCache::Clear();
}


// 创建笔刷
ID2D1Brush*	BrushCache::CreateBrush(const BrushCache::GameBrushParameter* pParameter){
	ID2D1SolidColorBrush* sbrush;
	ID2D1GradientStopCollection * stops;
	ID2D1LinearGradientBrush * lbrush;
	ID2D1RadialGradientBrush * rbrush;
	check_pointer(ppCacheRT, "BitmapCache::s_ppRenderTarget -> 0\n");
	check_pointer(*ppCacheRT, "*BitmapCache::s_ppRenderTarget -> 0\n");
	if (pParameter->stop_count < 2){
		// 创建纯色笔刷
#ifdef _DEBUG
		if (FAILED((*ppCacheRT)->CreateSolidColorBrush(pParameter->gradient_stops->color, &sbrush)))
			_cwprintf(L"纯色笔刷创建失败\n");
#else
		(*ppCacheRT)->CreateSolidColorBrush(pParameter->gradient_stops->color, &sbrush);
#endif
		return sbrush;
	}
	else{
		// 创建渐变停止点集合
		if (SUCCEEDED((*ppCacheRT)->CreateGradientStopCollection(pParameter->gradient_stops, pParameter->stop_count,
			D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &stops))){
			switch (pParameter->brush_type)
			{
			case BrushType::Linear:
				// 创建线性渐变笔刷
#ifdef _DEBUG
				if (FAILED((*ppCacheRT)->CreateLinearGradientBrush(pParameter->linear_properties, stops, &lbrush)))
					_cwprintf(L"创建线性渐变笔刷创建失败\n");
#else
				(*ppCacheRT)->CreateLinearGradientBrush(pParameter->linear_properties, stops, &lbrush);
#endif
				SafeRelease(stops);
				return lbrush;
			case BrushType::Radial:
			default:
				// 创建径向渐变笔刷
#ifdef _DEBUG
				if (FAILED((*ppCacheRT)->CreateRadialGradientBrush(pParameter->radial_properties, stops, &rbrush)))
					_cwprintf(L"创建径向渐变笔刷创建失败\n");
#else
				(*ppCacheRT)->CreateRadialGradientBrush(pParameter->radial_properties, stops, &rbrush);
#endif
				SafeRelease(stops);
				return rbrush;

			}
		}
#ifdef _DEBUG
		else
			_cwprintf(L"渐变停止点集合创建失败\n");
#endif
		SafeRelease(stops);
	}
	return nullptr;
}


#endif
#undef ppCacheRT