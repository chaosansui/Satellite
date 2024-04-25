// Fill out your copyright notice in the Description page of Project Settings.


#include "SatelliteHUD.h"
#include "Engine/Canvas.h"

void ASatelliteHUD::DrawHUD()
{
    Super::DrawHUD();


    // 获取画布对象
    if (Canvas)
    {
        
        // 设置盒子的位置和大小
        FVector2D BoxPosition = FVector2D(20, Canvas->SizeY - 20 - 6* 20); // 左下角起点
        FVector2D BoxSize = FVector2D(200, 6 * 20); // 宽度为400，高度为6行文本

        // 绘制边框盒子
        FCanvasBoxItem BorderBoxItem(BoxPosition - FVector2D(2, 2), BoxSize + FVector2D(4, 4)); // 相对盒子的边框向外扩展2像素
        BorderBoxItem.SetColor(FLinearColor::White);
        Canvas->DrawItem(BorderBoxItem);

        // 绘制内部盒子
        FCanvasBoxItem BoxItem(BoxPosition, BoxSize);
        BoxItem.SetColor(FLinearColor::Black);
        Canvas->DrawItem(BoxItem);

        // 设置文本的起始位置
        FVector2D TextPosition = BoxPosition + FVector2D(5, 5); // 文本起点位置在盒子内偏移一定距离

        // 使用中文字体
        UFont* Font = GEngine->GetLargeFont(); // 这里使用的是默认字体，你也可以加载其他支持中文的字体

        // 绘制文本
        FString Text1 = FString::Printf(TEXT("SatelliteLocaltionX:"));
        FCanvasTextItem TextItem1(TextPosition + FVector2D(0, 0), FText::FromString(Text1), Font, FLinearColor::White);
        Canvas->DrawItem(TextItem1);

        //需调用实际参数x坐标
        FString Location1 = FString::Printf(TEXT("xxxx"));
        FCanvasTextItem LocationItem1(TextPosition + FVector2D(120, 0), FText::FromString(Location1), Font, FLinearColor::White);
        Canvas->DrawItem(LocationItem1);

        FString Text2 = FString::Printf(TEXT("SatelliteLocaltionY:"));
        FCanvasTextItem TextItem2(TextPosition + FVector2D(0, 20), FText::FromString(Text2), GEngine->GetSmallFont(), FLinearColor::White);
        Canvas->DrawItem(TextItem2);
        
        //需调用实际参数y坐标
        FString Location2 = FString::Printf(TEXT("xxxx"));
        FCanvasTextItem LocationItem2(TextPosition + FVector2D(120, 20), FText::FromString(Location2), Font, FLinearColor::White);
        Canvas->DrawItem(LocationItem2);

        FString Text3 = FString::Printf(TEXT("SatelliteLocaltionZ:"));
        FCanvasTextItem TextItem3(TextPosition + FVector2D(0,40), FText::FromString(Text3), GEngine->GetSmallFont(), FLinearColor::White);
        Canvas->DrawItem(TextItem3);

        //需调用实际参数z坐标
        FString Location3 = FString::Printf(TEXT("xxx"));
        FCanvasTextItem LocationItem3(TextPosition + FVector2D(120, 40), FText::FromString(Location3), Font, FLinearColor::White);
        Canvas->DrawItem(LocationItem3);

        FString Text4 = FString::Printf(TEXT("Line 4"));
        FCanvasTextItem TextItem4(TextPosition + FVector2D(0, 60), FText::FromString(Text4), GEngine->GetSmallFont(), FLinearColor::White);
        Canvas->DrawItem(TextItem4);

        FString Text5 = FString::Printf(TEXT("Line 5"));
        FCanvasTextItem TextItem5(TextPosition + FVector2D(0, 80), FText::FromString(Text5), GEngine->GetSmallFont(), FLinearColor::White);
        Canvas->DrawItem(TextItem5);

        FString Text6 = FString::Printf(TEXT("Line 6"));
        FCanvasTextItem TextItem6(TextPosition + FVector2D(0, 100), FText::FromString(Text6), GEngine->GetSmallFont(), FLinearColor::White);
        Canvas->DrawItem(TextItem6);

    }
}