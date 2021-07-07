//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
typedef	struct	_COMMAND_PACKET	{	// �Զ����������ṹ
	UCHAR	mCommandCode;		// ����������,������Ķ���
	UCHAR	mCommandCodeNot;	// ������ķ���,����У�������
	union	{
		UCHAR	mParameter[5];	// ����
		struct {
			UCHAR	mBufferID;  // ������ʶ����,���������MCS51��Ƭ������: 1-ר�ù��ܼĴ���SFR, 2-�ڲ�RAM, 3-�ⲿRAM, ����������ʵ��ֻ��ʾ�ڲ�RAM
			USHORT	mBufferAddr;	// ��д��������ʼ��ַ,Ѱַ��Χ��0000H-0FFFFH,���ֽ���ǰ
			USHORT	mLength;	// ���ݿ��ܳ���,���ֽ���ǰ
		};
	};
}	mCOMMAND_PACKET,	*mpCOMMAND_PACKET;

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TBitBtn *btnCmd;
        TBitBtn *btnSend;
        TBitBtn *btnClose;
        TEdit *edtN1;
        TEdit *edtCmd;
    TButton *btnInt;
    TButton *btnRe;
    TLabel *Label3;
    TEdit *edtInt;
        void __fastcall btnSendClick(TObject *Sender);
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall btnCmdClick(TObject *Sender);
    void __fastcall edtN1KeyPress(TObject *Sender, char &Key);
    void __fastcall edtCmdKeyPress(TObject *Sender, char &Key);
    void __fastcall btnIntClick(TObject *Sender);
    void __fastcall btnReClick(TObject *Sender);

private:	// User declarations
public:
    char * mCaptionInform;
    ULONG mIndex;		// User declarations
    __fastcall TForm1(TComponent* Owner);
    UCHAR	mCharToBcd(UCHAR);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
#define		CONST_CMD_LEN		0x07	// �����ĳ���
#define		DEMO_CH451_CMD			0x56	// PC���������CH451,������ʾCH451�Ĺ���
#define		IDC_INT_PRESS		2222	// ���������¼��ź�
#define		IDC_INT_RELEASE		2223	// �����ͷ��¼��ź�

VOID	CALLBACK mInterruptEvent(PUCHAR);
//---------------------------------------------------------------------------
#endif
