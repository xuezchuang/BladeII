// Fill out your copyright notice in the Description page of Project Settings.


#include "B2UIToolTipTextBox.h"
#include "B2UIManager.h"




void UB2UIToolTipTextBox::Init()
{
	Super::Init();

	m_fClockDirectionFromTarget = 0;
	m_pTargetWidget = NULL;	

	m_fTimeToInit = -1;
}

void UB2UIToolTipTextBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateTooltipBox();

	if (m_fTimeToInit != -1 && GetWorld()->GetTimeSeconds() - m_fTimeToInit > m_fLifeTime)
	{
		UB2UIManager* pUIManager = UB2UIManager::GetInstance();
		if (pUIManager)
			pUIManager->CloseUI(UIFName::ToolTipTextBox);
	}
}

void UB2UIToolTipTextBox::SetContentText(FText txtContent)
{
	if (TB_Content.IsValid())
		TB_Content->SetText(txtContent);
}

void UB2UIToolTipTextBox::SetTargetInfo(UB2UIWidgetBase* TargetUB2UIWidget, UWidget* Target, float ClockDirection /*= 6.0f*/, float InLifeTime /*= 5.0f*/)
{
	m_pTargetUB2UIWidget = TargetUB2UIWidget;
	m_pTargetWidget = Target;
	m_fClockDirectionFromTarget = ClockDirection;

	m_fTimeToInit = GetWorld()->GetTimeSeconds();
	m_fLifeTime = InLifeTime;
}

// 浚柳努贰胶 扒靛妨具秦辑 焊幅 怖鞘夸窍搁 眠啊窍磊.
//void UB2UIToolTipTextBox::SetContentTextFontSize(uint16 FontSize)
//{
//	if (TB_Content.IsValid())
//		TB_Content->SetFontSize(FontSize);
//}

void UB2UIToolTipTextBox::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, CP_BG);
	GET_SLOT(UB2Image, IMG_BG);
	GET_SLOT(UOverlay, OverRay_Text);
	GET_SLOT(UB2RichTextBlock, TB_Content);
}

void UB2UIToolTipTextBox::UpdateTooltipBox()
{
	if (CP_BG.IsValid() && OverRay_Text.IsValid() && IMG_BG.IsValid())
	{
		UCanvasPanelSlot* pBGSlot = Cast<UCanvasPanelSlot>(CP_BG->Slot);

		FVector2D BGNewSize = OverRay_Text->GetDesiredSize();

		// 硅版捞固瘤 寇胞何盒栏肺 敬 农扁 滴硅父怒 农霸 棱酒淋.
		BGNewSize.X += (IMG_BG->GetBrush().ImageSize.X * IMG_BG->GetBrush().Margin.Left
			+ IMG_BG->GetBrush().ImageSize.X * IMG_BG->GetBrush().Margin.Right)
			* 2;

		BGNewSize.Y += (IMG_BG->GetBrush().ImageSize.Y * IMG_BG->GetBrush().Margin.Top
			+ IMG_BG->GetBrush().ImageSize.Y * IMG_BG->GetBrush().Margin.Bottom)
			* 2;

		pBGSlot->SetSize(BGNewSize);

		// 困摹汲沥. 鸥百 吝痢 - 郴吝痢 = 皋牢 牡滚胶菩澄 困摹
		// 郴吝痢 = 鸥百 吝痢 + 炼沥 蔼
		// 炼沥 蔼 = 澜.. 涝仿罐篮 规氢栏肺 氦磐2叼甫 父甸绢辑 弊氦磐甫 鸥百 啊肺尔 技肺尔 
		// 阿阿 X Y 厚啦 厚背秦辑 厚啦 歹 陋篮率 厚啦父怒 农扁甫 蚌秦林绊, 郴农扁档 鞍篮 累诀秦淋.
		// 酒 厚啦 蚌秦临订 荤捞令 例父馆怒父 蚌秦拎具且淀.
		
		if (m_pTargetUB2UIWidget && m_pTargetWidget)
		{
			FVector2D vTotalRect = FVector2D(0, 0);

			m_pTargetWidget->ForceLayoutPrepass();
			FVector2D vTargetPos = m_pTargetUB2UIWidget->GetWidgetScreenPos(m_pTargetWidget);

			UCanvasPanelSlot* pTargetWidgetSlot = Cast<UCanvasPanelSlot>(m_pTargetWidget->Slot);
			if (pTargetWidgetSlot)
			{
				vTargetPos = vTargetPos + (pTargetWidgetSlot->GetSize() / 2);

				// 鸥百 冠胶粱 农霸棱绰促. 呈公 迭嘿澜
				vTotalRect += pTargetWidgetSlot->GetSize() * 1.2;
			}			

			FVector2D vCenterPos;
			GetWorld()->GetGameViewport()->GetViewportSize(vCenterPos);
			vCenterPos = vCenterPos / 2;
			
			vTotalRect += Cast<UCanvasPanelSlot>(IMG_BG->Slot)->GetSize();

			// 困摹 焊沥
			FVector2D vDirection = FVector2D(0, -1);
			vDirection = vDirection.GetRotated((m_fClockDirectionFromTarget / 12) * 360);

			float fRateX = vDirection.X / vTotalRect.X;
			float fRateY = vDirection.Y / vTotalRect.Y;

			FVector2D vAdjustAmount;

			if (fRateX >= fRateY)
			{
				vAdjustAmount.X = vTotalRect.X;
				vAdjustAmount.Y = vTotalRect.Y * vDirection.Y;
			}
			else
			{
				vAdjustAmount.X = vTotalRect.X * vDirection.X;
				vAdjustAmount.Y = vTotalRect.Y;
			}			

			Cast<UCanvasPanelSlot>(CP_BG->Slot)->SetPosition(vTargetPos - vCenterPos + vAdjustAmount);
			Cast<UCanvasPanelSlot>(OverRay_Text->Slot)->SetPosition(vTargetPos - vCenterPos + vAdjustAmount);
		}
	}	
}
