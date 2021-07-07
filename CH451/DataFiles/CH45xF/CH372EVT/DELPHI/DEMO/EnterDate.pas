unit EnterDate;
interface 
uses Windows, SysUtils, Variants, Classes, Forms, StdCtrls, Controls;
type
  TEnterDlg = class(TForm)
    OKBtn: TButton;
    CancelBtn: TButton;
    Text1: TEdit;
    Label1: TLabel;   //���µİ�ť���
    procedure OKBtnClick(Sender: TObject);
    procedure CancelBtnClick(Sender: TObject);
  private
    { Private declarations }
  public

    { Public declarations }
  end;

var
  EnterDlg: TEnterDlg;
  index : Integer=0;   //���µİ�ť���
Const  DEMO_CH451_CMD= $56;

implementation
uses Ch375dll,unit1,Main ;
{$R *.dfm}
var
    mNumber : Byte;
    mBcdCode1:Byte;
    mLength:cardinal;
    mBcdCode:array[0..2] of Byte;

procedure TEnterDlg.OKBtnClick(Sender: TObject);
var
   ilen:Integer;
   mDemoReq:COMMAND_PACKET;
   BtDisChar:pchar;
begin
    mEnterBuffer := Text1.Text;
    If index <> -1 Then              //����ܶ�Ӧ�İ���
      begin
        If Text1.Text <> '' Then
          begin
            case mEnterBuffer[1] of
                 ' ':mBcdCode1:=$10; // �ո�
                 '=':mBcdCode1:=$13;
                 '.':mBcdCode1:=$1A
            Else
                mBcdCode1 := mCharToBcd(strtochar(strLeft(mEnterBuffer, 1)));  //ȡ�����ַ����������һλ
            END;
            ilen := Length(mEnterBuffer);
            If (ilen >= 2) And (strRight(strLeft(mEnterBuffer, 2), 1) = '.') Then  mBcdCode1 := (mBcdCode1 Or $80);
            If mBcdCode1 <> $FF Then            //������ַ���Ч
               begin
                mNumber := index;               // ���Bt1��Bt8����,����Ϊ0-7,�ֱ��Ӧ�ڸ�������ܰ�ť
                mNumber := 7 - mNumber;
                mDemoReq.mCommandCode := DEMO_CH451_CMD;
                mDemoReq.mCommandCodeNot := $FF - DEMO_CH451_CMD;
                mDemoReq.mParameter[2] := ($8 Or mNumber);      // CH451����:���������ݵ�ָ�������
                mDemoReq.mParameter[1] := mBcdCode1;            //������,��Ӧ��BCD���뷽ʽ
                mLength := CONST_CMD_LEN;    // ������ĳ���
                ////On Error Resume Next
                If CH375WriteData(mIndex, @mDemoReq, @mLength) Then    // ͨ��CH371��������,�ɹ�
                // �������ʱmLength����8,��ɹ�����ʱ,Ϊ�˸��ӿɿ�,���Լ��ʵ�ʷ��͵ĳ����Ƿ����
                    begin
                    //mEnterBuffer[2] := '0'; // �ַ���������,ֻҪһ���ַ�
                    //frmMain.Command1(index).Caption = Left(mEnterBuffer, 1)  //���ð�ť�ϵ��ַ�
                    Btdischar:=AnsiStrUpper(pchar(copy(mEnterBuffer,1,1)));
                    SetDlgItemText( frmmain.Handle , GetDisBtHwnd(index),Btdischar);
                    end
                Else
                   application.MessageBox('CH375WriteData ʧ�ܣ�','��Ϣ��ʾ',Mb_ok+MB_ICONEXCLAMATION);
               END
            Else
                application.MessageBox('�������ַ�0-9,A-F���߿ո�','��Ϣ��ʾ',Mb_ok+MB_ICONEXCLAMATION);
          END
        Else
            application.MessageBox('�������ַ�0-9,A-F���߿ո�','��Ϣ��ʾ',Mb_ok+MB_ICONEXCLAMATION);
        //index := 0;
        EXIT;  //�˳���ǰ����
      end
    Else
    begin
      if index=-1 then             //���ť����
      begin
        If (Length(mEnterBuffer) >= 3) Then    // �Ѿ���������,���ҳ��ȳ���3λ
          begin
            mBcdCode[0] := mCharToBcd(strtochar(strLeft(mEnterBuffer, 1)));
            mBcdCode[1] := mCharToBcd(strtochar(strRight(strLeft(mEnterBuffer, 2), 1)));
            mBcdCode[2] := mCharToBcd(strtochar(strRight(strLeft(mEnterBuffer, 3), 1)));
            If (mBcdCode[0] <> $FF) And (mBcdCode[1] <> $FF) And (mBcdCode[2] <> $FF) Then    // ����������ַ�����Ч
              begin
                mDemoReq.mCommandCode := DEMO_CH451_CMD;
                mDemoReq.mCommandCodeNot := $FF - DEMO_CH451_CMD;
                mDemoReq.mParameter[2] := mBcdCode[0];  //CH451����:��4λ
                mDemoReq.mParameter[1] := mBcdCode[1] * $10 Or mBcdCode[2]; // ��8λ���ֽ�����
                mLength := CONST_CMD_LEN;    // ������ĳ���
                If CH375WriteData(mIndex, @mDemoReq, @mLength) Then    // ͨ��CH375��������,�ɹ�
                    application.MessageBox('���ɹ����͵�CH451', '��Ϣ��ʾ',Mb_ok+MB_ICONEXCLAMATION)
                Else
                    application.MessageBox('CH375WriteData ʧ��','��Ϣ��ʾ',Mb_ok+MB_ICONEXCLAMATION);
              end
            Else
                application.MessageBox('������3���ַ�0-9,A-F','��Ϣ��ʾ',Mb_ok+MB_ICONEXCLAMATION);
          end
        Else
           begin
            application.MessageBox('����������3���ַ�0-9,A-F','��Ϣ��ʾ',Mb_ok+MB_ICONEXCLAMATION);
            exit;
           end;
      end;
  end;
end;

procedure TEnterDlg.CancelBtnClick(Sender: TObject);
begin
    mEnterBuffer := '' ;         //������ť
    exit;
end;

end.

