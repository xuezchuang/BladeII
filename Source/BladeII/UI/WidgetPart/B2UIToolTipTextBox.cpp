// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
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

// ����Ŭ���� �ǵ�����ؼ� ���� ���ʿ��ϸ� �߰�����.
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

		// ����̹��� �ܰ��κ����� �� ũ�� �ι踸ŭ ũ�� �����.
		BGNewSize.X += (IMG_BG->GetBrush().ImageSize.X * IMG_BG->GetBrush().Margin.Left
			+ IMG_BG->GetBrush().ImageSize.X * IMG_BG->GetBrush().Margin.Right)
			* 2;

		BGNewSize.Y += (IMG_BG->GetBrush().ImageSize.Y * IMG_BG->GetBrush().Margin.Top
			+ IMG_BG->GetBrush().ImageSize.Y * IMG_BG->GetBrush().Margin.Bottom)
			* 2;

		pBGSlot->SetSize(BGNewSize);

		// ��ġ����. Ÿ�� ���� - ������ = ���� ĵ�����г� ��ġ
		// ������ = Ÿ�� ���� + ���� ��
		// ���� �� = ��.. �Է¹��� �������� ����2�� ���� �׺��͸� Ÿ�� ���ζ� ���ζ� 
		// ���� X Y ���� ���ؼ� ���� �� ª���� ������ŭ ũ�⸦ �����ְ�, ��ũ�⵵ ���� �۾�����.
		// �� ���� �����ٶ� ������ ������ŭ�� ��������ҵ�.
		
		if (m_pTargetUB2UIWidget && m_pTargetWidget)
		{
			FVector2D vTotalRect = FVector2D(0, 0);

			m_pTargetWidget->ForceLayoutPrepass();
			FVector2D vTargetPos = m_pTargetUB2UIWidget->GetWidgetScreenPos(m_pTargetWidget);

			UCanvasPanelSlot* pTargetWidgetSlot = Cast<UCanvasPanelSlot>(m_pTargetWidget->Slot);
			if (pTargetWidgetSlot)
			{
				vTargetPos = vTargetPos + (pTargetWidgetSlot->GetSize() / 2);

				// Ÿ�� �ڽ��� ũ����´�. �ʹ� ������
				vTotalRect += pTargetWidgetSlot->GetSize() * 1.2;
			}			

			FVector2D vCenterPos;
			GetWorld()->GetGameViewport()->GetViewportSize(vCenterPos);
			vCenterPos = vCenterPos / 2;
			
			vTotalRect += Cast<UCanvasPanelSlot>(IMG_BG->Slot)->GetSize();

			// ��ġ ����
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
