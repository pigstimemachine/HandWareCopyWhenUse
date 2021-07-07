unit Unit1;
interface
Type
    COMMAND_PACKET=record     // �Զ����������ṹ
    mCommandCode : Byte;      // ����������,������Ķ���
    mCommandCodeNot : Byte;   // ������ķ���,����У�������
    mParameter:array[0..4] of Byte;   // ����
            //  mParamter(4)�൱�� UCHAR   mBufferID;  // ������ʶ����,���������MCS51��Ƭ������: 1-ר�ù��ܼĴ���SFR, 2-�ڲ�RAM, 3-�ⲿRAM, ����������ʵ��ֻ��ʾ�ڲ�RAM
            //  USHORT  mBufferAddr;    // ��д��������ʼ��ַ,Ѱַ��Χ��0000H-0FFFFH,���ֽ���ǰ
            //  USHORT  mLength;    // ���ݿ��ܳ���,���ֽ���ǰ
End;
type
     Tintbuffpkt=record
     buff1 : integer ;
     buff2 : integer ;
     buff3 : integer ;
     buff4 : integer ;
     buff5 : integer ;
     buff6 : integer ;
     buff7 : integer ;
     buff8 : integer ;
end;
var
      mCOMMAND_PACKET : COMMAND_PACKET ;
      mEnterBuffer : String;
      mBufferLength : cardinal;       // �������ڵ���Ч���ݳ���
      mShowLED : Integer=0;
      mIndex : cardinal=0;
      mKeyCode : Integer=$FF;
      mCaptionInform :array[0..7]of char='��Ϣ��ʾ';
      mDemoReq : COMMAND_PACKET;
      mOpen : Integer;
      index:integer;     //���µİ�ť���
      intbuff:Tintbuffpkt;
Const DEMO_GET_KEY = $34;
      DEMO_RET_OK = $0;
      CONST_CMD_LEN = $7;           // �����ĳ���
// �������������ݶ���ͨ�������´��ܵ�(USB�˵�2��OUT)�´�, Ϊ�˷�ֹ���߻���,
// ���ǿ����ڼ����Ӧ�ó����뵥Ƭ������֮��Լ��, �����ĳ�������7, �����ݿ�ĳ��ȿ϶�����7, ����64,32��
// ����, ����Լ��, ���������ֽ���������, �ȵ�
// ������Լ��������: 80H-0FFH��ͨ������,�����ڸ���Ӧ��
//                   00H-7FH��ר������,��Ը���Ӧ���ر���
// ͨ������
   DEF_CMD_GET_INFORM = $90;          // ��ȡ��λ����˵����Ϣ,���Ȳ�����64���ַ�,�ַ�����00H����
   DEF_CMD_TEST_DATA = $91;           // ��������,��λ����PC�����������������������ȡ���󷵻�
   DEF_CMD_CLEAR_UP = $A0;            // ���ϴ����ݿ�֮ǰ����ͬ��,ʵ��������λ������ϴ�����������������
   DEF_CMD_UP_DATA = $A1;             // ����λ����ָ����ַ�Ļ������ж�ȡ���ݿ�(�ϴ����ݿ�)
   DEF_CMD_DOWN_DATA = $A2;           // ����λ����ָ����ַ�Ļ�������д�����ݿ�(�´����ݿ�)
// ר������
   DEMO_CH451_CMD = $56;              // PC���������CH451,������ʾCH451�Ĺ���
// ����MCS51��Ƭ����ʹ��ͨ������ʱ,����Ҫָ��������ʶ����
   ACCESS_MCS51_SFR = 1;              // ��д51��Ƭ����SFR
   ACCESS_MCS51_IRAM = 2;             // ��д51��Ƭ�����ڲ�RAM
   ACCESS_MCS51_XRAM = 3;             // ��д51��Ƭ�����ⲿRAM

function strtochar(str:string):char;
function mCharToBcd(iChar:CHAR ):Byte;// ʮ������תBCD
function strLeft(str:shortstring ; count:integer):string;
function StrRight(str:string ; count:integer):string;
function GetDisBtHwnd(index:cardinal):cardinal;
function GetBtHwnd(KeyNo:cardinal):cardinal;      //��ȡ��Ӧ���Ű�ť���
procedure mSyncKeyboard(iKeyCode : cardinal);

implementation
uses main,Ch375dll,Windows,sysutils,enterdate;

procedure mSyncKeyboard(iKeyCode : cardinal);           // ����İ���ֵ,00H-3FH���������,0FFH���µļ����ͷ�
// ���·���ֻ������IDC_K0��IDC_K63��ȫ��������
begin
    If iKeyCode = $FF Then                              // �ͷŸհ��µļ�
         //Command2(mKeyNo).Caption = CStr(mKeyNo);     //�ָ���ʾ����
        SetDlgItemText( frmmain.Handle  , GetBtHwnd(mKeyNo), Pchar(inttostr(mKeyNo)))
    Else                                                // ��������
        begin
        mKeyNo := iKeyCode And $3F;                     // ����0-63
        SetDlgItemText( frmmain.Handle  , GetBtHwnd(mKeyNo), pchar('��') );  // 00H-3FH������������ʾ��
        End;
end;


 function GetDisBtHwnd(index:cardinal):cardinal;  //��ȡ��Ӧ���Ű�ť���
  begin
     case index of

                  0:GetDisBtHwnd:=FrmMain.Button1.Handle;
                  1:GetDisBtHwnd:=FrmMain.Button2.Handle;
                  2:GetDisBtHwnd:=FrmMain.Button3.Handle;
                  3:GetDisBtHwnd:=FrmMain.Button4.Handle;
                  4:GetDisBtHwnd:=FrmMain.Button5.Handle;
                  5:GetDisBtHwnd:=FrmMain.Button6.Handle;
                  6:GetDisBtHwnd:=FrmMain.Button7.Handle;
                  7:GetDisBtHwnd:=FrmMain.Button8.Handle
     else
          GetDisBtHwnd:=0;
     end;
end;

function GetBtHwnd(KeyNo:cardinal):cardinal;  //��ȡ��������Ӧ���Ű�ť���
  begin
     case keyno of
                  0:GetBtHwnd:=FrmMain.Button10.Handle;
                  1:GetBtHwnd:=FrmMain.Button11.Handle;
                  2:GetBtHwnd:=FrmMain.Button12.Handle;
                  3:GetBtHwnd:=FrmMain.Button13.Handle;
                  4:GetBtHwnd:=FrmMain.Button14.Handle;
                  5:GetBtHwnd:=FrmMain.Button15.Handle;
                  6:GetBtHwnd:=FrmMain.Button16.Handle;
                  7:GetBtHwnd:=FrmMain.Button17.Handle;
                  8:GetBtHwnd:=FrmMain.Button18.Handle;
                  9:GetBtHwnd:=FrmMain.Button19.Handle;
                  10:GetBtHwnd:=FrmMain.Button20.Handle;
                  11:GetBtHwnd:=FrmMain.Button21.Handle;
                  12:GetBtHwnd:=FrmMain.Button22.Handle;
                  13:GetBtHwnd:=FrmMain.Button23.Handle;
                  14:GetBtHwnd:=FrmMain.Button24.Handle;
                  15:GetBtHwnd:=FrmMain.Button25.Handle;
                  16:GetBtHwnd:=FrmMain.Button26.Handle;
                  17:GetBtHwnd:=FrmMain.Button27.Handle;
                  18:GetBtHwnd:=FrmMain.Button28.Handle;
                  19:GetBtHwnd:=FrmMain.Button29.Handle;
                  20:GetBtHwnd:=FrmMain.Button30.Handle;
                  21:GetBtHwnd:=FrmMain.Button31.Handle;
                  22:GetBtHwnd:=FrmMain.Button32.Handle;
                  23:GetBtHwnd:=FrmMain.Button33.Handle;
                  24:GetBtHwnd:=FrmMain.Button34.Handle;
                  25:GetBtHwnd:=FrmMain.Button35.Handle;
                  26:GetBtHwnd:=FrmMain.Button36.Handle;
                  27:GetBtHwnd:=FrmMain.Button37.Handle;
                  28:GetBtHwnd:=FrmMain.Button38.Handle;
                  29:GetBtHwnd:=FrmMain.Button39.Handle;
                  30:GetBtHwnd:=FrmMain.Button40.Handle;
                  31:GetBtHwnd:=FrmMain.Button41.Handle;
                  32:GetBtHwnd:=FrmMain.Button42.Handle;
                  33:GetBtHwnd:=FrmMain.Button43.Handle;
                  34:GetBtHwnd:=FrmMain.Button44.Handle;
                  35:GetBtHwnd:=FrmMain.Button45.Handle;
                  36:GetBtHwnd:=FrmMain.Button46.Handle;
                  37:GetBtHwnd:=FrmMain.Button47.Handle;
                  38:GetBtHwnd:=FrmMain.Button48.Handle;
                  39:GetBtHwnd:=FrmMain.Button49.Handle;
                  40:GetBtHwnd:=FrmMain.Button50.Handle;
                  41:GetBtHwnd:=FrmMain.Button51.Handle;
                  42:GetBtHwnd:=FrmMain.Button52.Handle;
                  43:GetBtHwnd:=FrmMain.Button53.Handle;
                  44:GetBtHwnd:=FrmMain.Button54.Handle;
                  45:GetBtHwnd:=FrmMain.Button55.Handle;
                  46:GetBtHwnd:=FrmMain.Button56.Handle;
                  47:GetBtHwnd:=FrmMain.Button57.Handle;
                  48:GetBtHwnd:=FrmMain.Button58.Handle;
                  49:GetBtHwnd:=FrmMain.Button59.Handle;
                  50:GetBtHwnd:=FrmMain.Button60.Handle;
                  51:GetBtHwnd:=FrmMain.Button61.Handle;
                  52:GetBtHwnd:=FrmMain.Button62.Handle;
                  53:GetBtHwnd:=FrmMain.Button63.Handle;
                  54:GetBtHwnd:=FrmMain.Button64.Handle;
                  55:GetBtHwnd:=FrmMain.Button65.Handle;
                  56:GetBtHwnd:=FrmMain.Button66.Handle;
                  57:GetBtHwnd:=FrmMain.Button67.Handle;
                  58:GetBtHwnd:=FrmMain.Button68.Handle;
                  59:GetBtHwnd:=FrmMain.Button69.Handle;
                  60:GetBtHwnd:=FrmMain.Button70.Handle;
                  61:GetBtHwnd:=FrmMain.Button71.Handle;
                  62:GetBtHwnd:=FrmMain.Button72.Handle;
                  63:GetBtHwnd:=FrmMain.Button73.Handle;
     else
                     GetBtHwnd:=0;
  end;
end;

function strtochar(str:string):char;
begin
    strtochar:=str[1];
end;

Function mCharToBcd(iChar:CHAR ):Byte; // ʮ��������תBCD��
var mBCD:Byte;
begin
    case iChar of
          '0'..'9': mBCD:=(ord(iChar) - ord('0'));
          'A'..'F': mBCD:=(ord(iChar) - ord('A') + $A);
          'a'..'f': mBCD:=(ord(iChar) - ord('a') + $A)
    Else
          mBCD:= $FF;
    end;
    mCharToBcd:= mBCD;
end;

function strLeft(str:shortstring ; count:integer):string; //��ȡ�ַ�
var res:string;
begin
         res:=copy( str,1,count);
         strleft:=res;
end;
Function StrRight(str:string ; count:integer):string;     //��ȡ�ַ�
var res:string;
begin
         res:=copy( str,length(str)-count+1,count);
         strright:=res;
end;

//�´���������
{
function mDownloadData(iBuffer:array of byte; iLength:cardinal ):cardinal;
var
    mDemoReq:COMMAND_PACKET;
    mLength:cardinal;
begin
    if ( iLength > 4096 ) then
    begin
      Messagebox( FrmMain.Handle, '�����´����ݳ��ȳ���4096�ֽ�', mCaptionInform, MB_OK+MB_ICONERROR );
      mDownloadData:=0;
      exit;
    end;
    mDemoReq.mCommandCode:= DEF_CMD_DOWN_DATA;	// �����´����ݿ�
    mDemoReq.mCommandCodeNot := $FF- DEF_CMD_DOWN_DATA;
    mDemoReq.mParameter[0] := ACCESS_MCS51_XRAM; // ��д51��Ƭ�����ⲿRAM(����ʾ��ĵ�Ƭ��û���ⲿRAM,�����޷���ʾ)
    mDemoReq.mParameter[1] := 00;	// ָ���ⲿRAM��������ʼ��ַ,�����ǽ������´�����ʼ��ַΪ0X8200���ⲿRAM
    mDemoReq.mParameter[2] :=$82;
    mDemoReq.mParameter[3] := iLength mod 256;  // ���������ܳ��ȵͶ�λ
    mDemoReq.mParameter[4] := iLength div 256;
    mLength := CONST_CMD_LEN;	// ������ĳ���
    if ( CH375WriteData( mIndex, @mDemoReq, @mLength ) ) then // ͨ��CH375���������,�ɹ�
    begin
      mLength := iLength;	// ���ݿ�ĳ���,һ���´�������4096�ֽ�
      if ( ( mLength mod 64 ) = CONST_CMD_LEN ) then mLength:=mLength+1;  // ��ֹ���ݰ��ĳ�����������ĳ�����ͬ,�����ͬ,��෢��һ����Ч����
      if ( CH375WriteData( mIndex, @mDemoReq, @mLength ) ) then // ͨ��CH375��������,�ɹ�
          mDownloadData:=mLength
      else MessageBox( frmmain.Handle, 'CH375WriteData �´�����ʧ��', mCaptionInform, MB_OK + MB_ICONERROR );
    end
    else MessageBox( frmmain.Handle, 'CH375WriteData ��������ʧ��,DEF_CMD_DOWN_DATA', mCaptionInform, MB_OK + MB_ICONERROR );
         mDownloadData:=0;
end;

//�ϴ���������
Function mUploadData( iBuffer:array of byte;iLength:cardinal ):cardinal;
var
	mDemoReq:COMMAND_PACKET ;
	mLength:cardinal;
begin
     if ( iLength > 4096 ) then
        MessageBox( frmmain.Handle, '�����ϴ����ݳ��ȳ���4096�ֽ�', mCaptionInform, MB_OK + MB_ICONERROR );
	mDemoReq.mCommandCode := DEF_CMD_CLEAR_UP;	// �����ϴ����ݿ�֮ǰ����ͬ��,ʵ�����õ�Ƭ������ϴ�����������������
	mDemoReq.mCommandCodeNot :=$FF-DEF_CMD_CLEAR_UP;
	mLength := CONST_CMD_LEN;	// ������ĳ���
	if ( CH375WriteData( mIndex, @mDemoReq, @mLength ) ) then // ͨ��CH375���������,�ɹ�
	  begin
	    mDemoReq.mCommandCode := DEF_CMD_UP_DATA;	// �����ϴ����ݿ�
	    mDemoReq.mCommandCodeNot := $FF- DEF_CMD_UP_DATA;
	    mDemoReq.mParameter[0]  := ACCESS_MCS51_XRAM; // ��д51��Ƭ�����ⲿRAM(����ʾ��ĵ�Ƭ��û���ⲿRAM,�����޷���ʾ)
            mDemoReq.mParameter[1] := 00;	// ָ���ⲿRAM��������ʼ��ַ,�����ǽ������´�����ʼ��ַΪ0X8200���ⲿRAM
            mDemoReq.mParameter[2] :=$82;
            mDemoReq.mParameter[3] := iLength mod 256;  // ���������ܳ��ȵͶ�λ
            mDemoReq.mParameter[4] := iLength div 256;
	    mLength := CONST_CMD_LEN;	// ������ĳ���
		if ( CH375WriteData( mIndex, @mDemoReq, @mLength ) ) then // ͨ��CH375���������,�ɹ�
		  begin
			mLength := iLength;	// ���ݿ�ĳ���,һ���ϴ�������4096�ֽ�
			if ( CH375ReadData( mIndex, @iBuffer, @mLength ) )  then// ͨ��CH375��������,�ɹ�
			    mUploadData := mLength
			else MessageBox( frmmain.Handle, 'CH375ReadData �ϴ�����ʧ��', mCaptionInform, MB_OK + MB_ICONERROR );
		  end
		else MessageBox( frmmain.Handle, 'CH375WriteData ��������ʧ��,DEF_CMD_UP_DATA', mCaptionInform, MB_OK + MB_ICONERROR );
	  end
	else MessageBox( frmmain.Handle, 'CH375WriteData ��������ʧ��,DEF_CMD_CLEAR_UP', mCaptionInform, MB_OK + MB_ICONERROR );
	  mUploadData:= 0;
End;
}

end.




