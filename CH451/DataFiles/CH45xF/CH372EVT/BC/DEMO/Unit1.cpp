//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "Unit1.h"
#include "CH375DLL.H"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
UINT	mKeyCode = 0xff;		// �������
HWND        mSaveDialogMain;	// ���Ի���

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
    mCaptionInform = " ��Ϣ��ʾ ";
    mIndex = 0;
    edtN1->Text = "";
    edtCmd->Text = "";
    edtInt->Text = "";
    mSaveDialogMain = this->Handle;

    if ( LoadLibrary( "CH375DLL.DLL" ) == NULL )  // ����DLLʧ��,����δ��װ��ϵͳ��
    {
        Application->MessageBox("�޷�����CH375��DLL", "information", MB_ICONSTOP | MB_OK );
    }
    else if ( CH375OpenDevice(0) == INVALID_HANDLE_VALUE ) // ʹ��֮ǰ������豸
    {
        Application->MessageBox("�޷���CH375�豸", "information", MB_ICONSTOP | MB_OK );
    }
    CH375SetTimeout( mIndex, 3000, 3000 );  // ����USB���ݶ�д�ĳ�ʱ,����3000mSδ��ɶ�д��ǿ�Ʒ���,����һֱ�ȴ���ȥ
    CH375SetExclusive( mIndex, FALSE );  // ��ʱ���ù���ʹ�õ�ǰCH375�豸,��Ϊ���������жϷ������ʱ��Ҫʹ�õ�ǰ���CH375�豸

    // ��������жϷ������,�жϷ����������Ӧ�ò�ִ�е�,���߳����ȼ���THREAD_PRIORITY_TIME_CRITICAL
    // ����Ƭ��������Ҫ֪ͨ�����ʱ,������CMD_WR_USB_DATA5����д���ж���������,�������mInterruptEvent�߳̽����յ����ж���������
    // Ȼ��mInterruptEvent�߳��������򷢳���Ϣ���д���,mInterruptEvent�߳��൱���жϷ������,����������ʱ��ѯ��Ƭ��
    CH375SetIntRoutine( mIndex, mInterruptEvent );

    CH375SetExclusive( mIndex, TRUE );  // ���ö�ռʹ�õ�ǰCH375�豸,�ڴ�֮������Ӧ�ó�����ͬʱʹ�õ�ǰ���CH375�豸

}

//---------------------------------------------------------------------------
void __fastcall TForm1::btnSendClick(TObject *Sender)
{
    mCOMMAND_PACKET mDemoReq;
    UINT 		mLength;
    UCHAR       mEnterBuffer[8];
    if(edtN1->GetTextLen()>0)
    {
        UCHAR	mBcdCode;
        UCHAR	mNumber;
        strcpy(mEnterBuffer,edtN1->Text.c_str());
        if ( mEnterBuffer[0] == ' ' ) mBcdCode = 0x10;  // �ո�
        else if ( mEnterBuffer[0] == '=' ) mBcdCode = 0x13;
        else if ( mEnterBuffer[0] == '.' ) mBcdCode = 0x1a;
        else mBcdCode = mCharToBcd( mEnterBuffer[0] );
        // �������һ���ַ���һ��С����,�����ַ����½�׷��С����
        if ( edtN1->GetTextLen() >= 2 && mEnterBuffer[1] == '.' ) mBcdCode |= 0x80;
        if ( mBcdCode != 0xff )  // ������ַ���Ч
        {
            mNumber = 7;  // �����λΪ��,���λΪ��,����N1��Ӧλ7,N8��Ӧ0
            mDemoReq.mCommandCode = DEMO_CH451_CMD;
            mDemoReq.mCommandCodeNot = ~ DEMO_CH451_CMD;
            mDemoReq.mParameter[2] = 0x08 | mNumber;  // CH451����:���������ݵ�ָ�������
            mDemoReq.mParameter[1] = mBcdCode;  // ������,��Ӧ��BCD���뷽ʽ
            mLength = CONST_CMD_LEN;	// ������ĳ���
            if ( CH375WriteData( mIndex, (void *)&mDemoReq, (unsigned long *)&mLength ) )  // ͨ��CH375��������,�ɹ�
// �������ʱmLength����64,��ɹ�����ʱ,Ϊ�˸��ӿɿ�,���Լ��ʵ�ʷ��͵ĳ����Ƿ����
            {
                mEnterBuffer[1] = 0;  // �ַ���������,ֻҪһ���ַ�
             //   SetDlgItemText( hDialog, LOWORD( wParam ), mEnterBuffer );   //���ð�ť�ϵ��ַ�
            }
            else Application->MessageBox( "CH375WriteData ʧ��", mCaptionInform, MB_OK | MB_ICONERROR );
        }
        else Application->MessageBox( "�������ַ�0-9,A-F���߿ո�", mCaptionInform, MB_OK | MB_ICONERROR );
    }
    else
        Application->MessageBox("�������ַ�0-9,A-F���߿ո�",mCaptionInform,MB_OK|MB_ICONSTOP);
    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnCloseClick(TObject *Sender)
{
    CH375CloseDevice( mIndex );
    this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnCmdClick(TObject *Sender)
{
    mCOMMAND_PACKET mDemoReq;
    UINT 		mLength;
    UCHAR       mEnterBuffer[8];
  //  UINT	mBufferLength = 0;
    if ( edtCmd->GetTextLen() >= 3 )  // �Ѿ���������,���ҳ��ȳ���3λ
    {
        UCHAR	mBcdCode[3];
        strcpy(mEnterBuffer,edtCmd->Text.c_str());
        mBcdCode[0] = mCharToBcd( mEnterBuffer[0] );
        mBcdCode[1] = mCharToBcd( mEnterBuffer[1] );
        mBcdCode[2] = mCharToBcd( mEnterBuffer[2] );
        if ( mBcdCode[0] != 0xff && mBcdCode[1] != 0xff && mBcdCode[2] != 0xff )  // ����������ַ�����Ч
        {
            mDemoReq.mCommandCode = DEMO_CH451_CMD;
            mDemoReq.mCommandCodeNot = ~ DEMO_CH451_CMD;
            mDemoReq.mParameter[2] = mBcdCode[0];  // CH451����:��4λ
            mDemoReq.mParameter[1] = mBcdCode[1] << 4 | mBcdCode[2];  // ��8λ���ֽ�����
            mLength = CONST_CMD_LEN;	// ������ĳ���
            if ( CH375WriteData( mIndex, (void *)&mDemoReq, (unsigned long *)&mLength ) )  // ͨ��CH375��������,�ɹ�
                Application->MessageBox( "����ɹ����͵�CH451", mCaptionInform, MB_OK );
            else Application->MessageBox( "CH375WriteData ʧ��", mCaptionInform, MB_OK | MB_ICONERROR );
        }
        else Application->MessageBox( "������3���ַ�0-9,A-F", mCaptionInform, MB_OK | MB_ICONERROR );
    }
    else Application->MessageBox( "����������3���ַ�0-9,A-F", mCaptionInform, MB_OK | MB_ICONERROR );
}
//---------------------------------------------------------------------------

UCHAR	TForm1::mCharToBcd(	UCHAR	iChar )  // �����ASCII�ַ�
{
	UCHAR	mBCD;
	if ( iChar >= '0' && iChar <= '9' ) mBCD = iChar -'0';
	else if ( iChar >= 'A' && iChar <= 'F' ) mBCD = iChar - 'A' + 0x0a;
	else if ( iChar >= 'a' && iChar <= 'f' ) mBCD = iChar - 'a' + 0x0a;
	else mBCD = 0xff;
	return( mBCD );
}
void __fastcall TForm1::edtN1KeyPress(TObject *Sender, char &Key)
{
    if(Key == 13)
        this->btnSend->Click();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::edtCmdKeyPress(TObject *Sender, char &Key)
{
    if(Key == 13)
        this->btnCmd->Click();
}
//---------------------------------------------------------------------------

VOID	CALLBACK	mInterruptEvent(  // �жϷ������
	PUCHAR			iBuffer )  // ָ��һ��������,�ṩ��ǰ���ж���������
{

// CH375�����жϺ�,����һ����Ϣ֪ͨ������
	if ( iBuffer[0] == 1 )  // ����ж���������
	{
		mKeyCode = iBuffer[1] & 0x3f;  // ���صļ�ֵ,��Ƭ��������ж��������ݻ������ĵ�2�ֽ�
		Form1->btnInt->Click();
	}
	else if ( iBuffer[0] == 2 )
        Form1->btnRe->Click();
   
// ���ʹ���ж���������,����ݸ���������Ӧ�Ĵ���
/*
	switch( iBuffer[0] )
	{
		case 1: �ж���������Ϊ1
		case 2: �ж���������Ϊ2
		.....
		case 6: �ж���������Ϊ6
	}
*/
}
void __fastcall TForm1::btnIntClick(TObject *Sender)
{
    UCHAR buf[8];
   // sprintf(buf,"02x%",mKeyCode);
 //   SetDlgItemText(this,edtInt,buf);
    edtInt->Text=AnsiString(mKeyCode+1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnReClick(TObject *Sender)
{
    edtInt->Text = "0";
}
//---------------------------------------------------------------------------

