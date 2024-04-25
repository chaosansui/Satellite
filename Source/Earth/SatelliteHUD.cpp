// Fill out your copyright notice in the Description page of Project Settings.


#include "SatelliteHUD.h"
#include "Engine/Canvas.h"

void ASatelliteHUD::DrawHUD()
{
    Super::DrawHUD();


    // ��ȡ��������
    if (Canvas)
    {
        
        // ���ú��ӵ�λ�úʹ�С
        FVector2D BoxPosition = FVector2D(20, Canvas->SizeY - 20 - 6* 20); // ���½����
        FVector2D BoxSize = FVector2D(200, 6 * 20); // ���Ϊ400���߶�Ϊ6���ı�

        // ���Ʊ߿����
        FCanvasBoxItem BorderBoxItem(BoxPosition - FVector2D(2, 2), BoxSize + FVector2D(4, 4)); // ��Ժ��ӵı߿�������չ2����
        BorderBoxItem.SetColor(FLinearColor::White);
        Canvas->DrawItem(BorderBoxItem);

        // �����ڲ�����
        FCanvasBoxItem BoxItem(BoxPosition, BoxSize);
        BoxItem.SetColor(FLinearColor::Black);
        Canvas->DrawItem(BoxItem);

        // �����ı�����ʼλ��
        FVector2D TextPosition = BoxPosition + FVector2D(5, 5); // �ı����λ���ں�����ƫ��һ������

        // ʹ����������
        UFont* Font = GEngine->GetLargeFont(); // ����ʹ�õ���Ĭ�����壬��Ҳ���Լ�������֧�����ĵ�����

        // �����ı�
        FString Text1 = FString::Printf(TEXT("SatelliteLocaltionX:"));
        FCanvasTextItem TextItem1(TextPosition + FVector2D(0, 0), FText::FromString(Text1), Font, FLinearColor::White);
        Canvas->DrawItem(TextItem1);

        //�����ʵ�ʲ���x����
        FString Location1 = FString::Printf(TEXT("xxxx"));
        FCanvasTextItem LocationItem1(TextPosition + FVector2D(120, 0), FText::FromString(Location1), Font, FLinearColor::White);
        Canvas->DrawItem(LocationItem1);

        FString Text2 = FString::Printf(TEXT("SatelliteLocaltionY:"));
        FCanvasTextItem TextItem2(TextPosition + FVector2D(0, 20), FText::FromString(Text2), GEngine->GetSmallFont(), FLinearColor::White);
        Canvas->DrawItem(TextItem2);
        
        //�����ʵ�ʲ���y����
        FString Location2 = FString::Printf(TEXT("xxxx"));
        FCanvasTextItem LocationItem2(TextPosition + FVector2D(120, 20), FText::FromString(Location2), Font, FLinearColor::White);
        Canvas->DrawItem(LocationItem2);

        FString Text3 = FString::Printf(TEXT("SatelliteLocaltionZ:"));
        FCanvasTextItem TextItem3(TextPosition + FVector2D(0,40), FText::FromString(Text3), GEngine->GetSmallFont(), FLinearColor::White);
        Canvas->DrawItem(TextItem3);

        //�����ʵ�ʲ���z����
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