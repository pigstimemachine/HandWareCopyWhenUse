unit Main;

interface

uses
  Windows, SysUtils, Variants, Classes, Forms, StdCtrls, Controls;

type
  TFrmMain = class(TForm)


    Label1: TLabel;
    Label2: TLabel;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    Button6: TButton;
    Button7: TButton;
    Button8: TButton;
    Button9: TButton;
    Button10: TButton;
    Button11: TButton;
    Button12: TButton;
    Button13: TButton;
    Button14: TButton;
    Button15: TButton;
    Button16: TButton;
    Button17: TButton;
    Button18: TButton;
    Button19: TButton;
    Button20: TButton;
    Button21: TButton;
    Button22: TButton;
    Button23: TButton;
    Button24: TButton;
    Button25: TButton;
    Button26: TButton;
    Button27: TButton;
    Button28: TButton;
    Button29: TButton;
    Button30: TButton;
    Button31: TButton;
    Button32: TButton;
    Button33: TButton;
    Button34: TButton;
    Button35: TButton;
    Button36: TButton;
    Button37: TButton;
    Button38: TButton;
    Button39: TButton;
    Button40: TButton;
    Button41: TButton;
    Button42: TButton;
    Button43: TButton;
    Button44: TButton;
    Button45: TButton;
    Button46: TButton;
    Button47: TButton;
    Button48: TButton;
    Button49: TButton;
    Button50: TButton;
    Button51: TButton;
    Button52: TButton;
    Button53: TButton;
    Button54: TButton;
    Button55: TButton;
    Button56: TButton;
    Button57: TButton;
    Button58: TButton;
    Button59: TButton;
    Button60: TButton;
    Button61: TButton;
    Button62: TButton;
    Button63: TButton;
    Button64: TButton;
    Button65: TButton;
    Button66: TButton;
    Button67: TButton;
    Button68: TButton;
    Button69: TButton;
    Button70: TButton;
    Button71: TButton;
    Button72: TButton;
    Button73: TButton;
    devstatue: TLabel;
    pnpproce: TButton;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure Button7Click(Sender: TObject);
    procedure Button8Click(Sender: TObject);
    procedure Button9Click(Sender: TObject);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure FormKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure pnpproceKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    //function mInterruptEvent(mBuffer:pbytearray):cardinal;                //�жϷ������
    //procedure mCallInt();
  private
    { Private declarations }
  public

    { Public declarations }
  end;

var
  FrmMain: TFrmMain;
  mKeyNo :cardinal=0;  //ģ�ⰴ����
  const WM_KEYDOWN=$100;
        WM_KEYUP=$101;

implementation
uses CH375dll,unit1,EnterDate;
{$R *.dfm}

//iBuffer ָ��һ��������,�ṩ��ǰ���ж���������
//******����˳���Ҫ���öϵ�******
 // �жϷ������
procedure mInterruptEvent(mBuffer:pbytearray );stdcall;  // ָ��һ��������,�ṩ��ǰ���ж���������
begin
    // CH371�����жϺ�,����һ����Ϣ֪ͨ������
    If mBuffer[0] = 1 Then        // ����ж���������
        begin
        mKeyCode := mBuffer[1] And $3F; //ȡ��ֵ
        PostMessage(frmmain.Handle, $100, 1, 0); // �ж���������Ϊ1����Ӧ���Ű�ť������
        end
    Else
        If mBuffer[0] = 2 Then
         PostMessage(frmmain.Handle, $101, 1, 0); // �ж���������Ϊ2����Ӧ���Ű������ͷ�
end;

procedure mCallInt() ;
var  x:boolean;
begin
    x:= CH375SetIntRoutine(mIndex,mInterruptEvent);    //�����ж��ӳ���
    If x =false Then
    begin
        MessageBox(frmmain.Handle,'�޷������ж�', mCaptionInform, MB_OK + MB_ICONERROR );
        Exit;
    End;
end;

// ���²���PC���뵥Ƭ��֮���USBͨѶ,������ʾ,����û������
procedure TestCommunication();
var     mLength : cardinal;
begin

    mDemoReq.mCommandCode := DEF_CMD_TEST_DATA;    // ��������,��PC����������������ȡ���󷵻�
    mDemoReq.mCommandCodeNot := $FF - DEF_CMD_TEST_DATA;
    mDemoReq.mParameter[0] := $5A;  // ����Ĳ�������,���غ󽫰�λȡ��
    mDemoReq.mParameter[1] := $96;  // ����Ĳ�������,���غ󽫰�λȡ��
    mDemoReq.mParameter[2] := $F3;  // ����Ĳ�������,���غ󽫰�λȡ��
    mDemoReq.mParameter[3] := $4C;  // ����Ĳ�������,���غ󽫰�λȡ��
    mDemoReq.mParameter[4] := $39;  // ����Ĳ�������,���غ󽫰�λȡ��
    mLength := CONST_CMD_LEN;       // ������ĳ���
    If CH375WriteData(mIndex,@mDemoReq,@mLength) Then        // ͨ��CH375������������,�ɹ�
    begin
      mLength := mCH375_PACKET_LENGTH;
      If (CH375ReadData(mIndex,@mDemoReq, @mLength)) Then        // ͨ��CH375����Ӧ������,�ɹ�
        begin
          If (mLength = CONST_CMD_LEN) Then
            begin
              If (mDemoReq.mCommandCode <> ($FF - DEF_CMD_TEST_DATA))
                 Or (mDemoReq.mParameter[0] <> ($FF - $5A))
                 Or (mDemoReq.mParameter[1] <> ($FF - $96)) Then
                  application.MessageBox ('ͨ��USB����������д���', mCaptionInform, Mb_ok+ MB_ICONEXCLAMATION);
            end
          Else  application.MessageBox ( 'CH375���ݲ��Է��صĳ��ȴ���',  mCaptionInform,mb_ok+ MB_ICONEXCLAMATION);
        end
      Else  application.MessageBox ( 'CH375ReadData ʧ��',  mCaptionInform,mb_ok+ MB_ICONEXCLAMATION);
    end
    Else  application.MessageBox ( 'CH375WriteData ʧ��',  mCaptionInform,mb_ok+ MB_ICONEXCLAMATION);

end;

//�豸�¼�֪ͨ�ص�����,���¼���Ϣ���͵�ָ���ؼ�����Ϣ��������д���.
Procedure NotifyRoutine(iEventStatus:cardinal );stdcall;
begin
    PostMessage(frmmain.pnpproce.Handle ,WM_KEYUP,iEventStatus,0);
end;

//����¼��������
procedure TFrmMain.pnpproceKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
var
    iEventStatus:cardinal;
    statue:longbool;
begin
    iEventStatus := Key;   //����¼�����
    statue:=FALSE;
    If (iEventStatus = CH375_DEVICE_ARRIVAL) Then
    begin
        if(mOpen=-1) then //�豸��û��
        begin
            mOpen := CH375OpenDevice(mIndex);                  //�������ʱ���豸
            If (mOpen = -1) Then
            begin
                Application.MessageBox('�޷���CH375�豸', '��Ϣ��ʾ',0);
                Exit;
            End;
            CH375SetTimeout( mIndex, 3000, 3000 );  // ����USB���ݶ�д�ĳ�ʱ,����3000mSδ��ɶ�д��ǿ�Ʒ���,����һֱ�ȴ���ȥ
            TestCommunication();
            mCallInt ;  //�����жϳ���
        end;
        frmmain.devstatue.Caption := '**CH372/CH375�豸�Ѳ���';
        statue:=TRUE;
    end
    Else
    begin
        If (iEventStatus = CH375_DEVICE_REMOVE) Then
        begin
            if(mOpen<>-1) then
            begin
                CH375CloseDevice (mIndex);     //�豸�γ�,�ر��豸���
                mOpen :=-1;
            end;
            devstatue.Caption := '**CH372/CH375�豸�Ѱγ�';
            statue:=FALSE;
        End;
    end;
    //�豸��,��ť����;�豸û��,��ť����
    //LED1...LED8
    frmmain.Button1.Enabled := statue;
    frmmain.Button2.Enabled := statue;
    frmmain.Button3.Enabled := statue;
    frmmain.Button4.Enabled := statue;
    frmmain.Button5.Enabled := statue;
    frmmain.Button6.Enabled := statue;
    frmmain.Button7.Enabled := statue;
    frmmain.Button8.Enabled := statue;
    //���ť
    frmmain.Button9.Enabled := statue;
end;

procedure TFrmMain.FormCreate(Sender: TObject);

begin
    mCaptionInform:= '��Ϣ��ʾ';

    //�����豸��μ���
    if(CH375SetDeviceNotify(mIndex,'',NotifyRoutine)=FALSE) then
        Application.MessageBox(  '���ü���CH372/CH375�豸���ʧ��', mCaptionInform, 0);

    mOpen := CH375OpenDevice(mIndex);                  //�������ʱ���豸
    If mOpen = -1 Then
    begin
      //application.MessageBox( '�޷���CH375�豸',mCaptionInform,MB_OK + MB_ICONEXCLAMATION);
      //halt(0);    //�رճ���
      NotifyRoutine( CH375_DEVICE_REMOVE);  //���豸�γ�����
      exit;
    End;



    CH375SetTimeout( mIndex, 3000, 3000 );  // ����USB���ݶ�д�ĳ�ʱ,����3000mSδ��ɶ�д��ǿ�Ʒ���,����һֱ�ȴ���ȥ

    //����PC���뵥Ƭ��֮���USBͨѶ,������ʾ,����û������
    TestCommunication();

//��������жϷ������ , �жϷ����������Ӧ�ò�ִ�е�, ���߳����ȼ���THREAD_PRIORITY_TIME_CRITICAL
//����Ƭ��������Ҫ֪ͨ�����ʱ , ������CMD_WR_USB_DATA5����д���ж���������, �������mInterruptEvent�߳̽����յ����ж���������
//Ȼ��mInterruptEvent�߳��������򷢳���Ϣ���д���,mInterruptEvent�߳��൱���жϷ������,����������ʱ��ѯ��Ƭ��

    mCallInt ;  //�����жϳ���
    NotifyRoutine(CH375_DEVICE_ARRIVAL);    //���豸���봦��
end;

procedure TFrmMain.FormClose(Sender: TObject; var Action: TCloseAction);
begin
   CH375SetDeviceNotify(mIndex,'',mPCH375_NOTIFY_ROUTINE(0));     //ȡ��Ϣ�豸ͦ��֪ͨ
   CH375SetIntRoutine(mIndex,TiIntRoutine(0));                    //ȡ���ж��ϴ�
   CH375CloseDevice (mIndex);       //�����˳�ǰ�ر��豸
   halt(0);
end;

procedure TFrmMain.FormDestroy(Sender: TObject);
begin
    CH375CloseDevice (mIndex);  //�����жϹر��豸
end;

procedure TFrmMain.Button1Click(Sender: TObject);  //����ܰ�ť1
var
  f: TEnterDlg;
begin
  index:=0;
  f := TEnterDlg.Create(Self);  //�����������ʾ
  f.ShowModal;
end;

procedure TFrmMain.Button2Click(Sender: TObject);  //����ܰ�ť2
var
  f: TEnterDlg;
begin
  index:=1;
  f := TEnterDlg.Create(Self);  //�����������ʾ
  f.ShowModal;
end;

procedure TFrmMain.Button3Click(Sender: TObject);   //����ܰ�ť3
var
  f: TEnterDlg;
begin
  index:=2;
  f := TEnterDlg.Create(Self);  //�����������ʾ
  f.ShowModal;

end;

procedure TFrmMain.Button4Click(Sender: TObject); //����ܰ�ť4
var
  f: TEnterDlg;
begin
  index:=3;
  f := TEnterDlg.Create(Self);  //�����������ʾ
  f.ShowModal;
end;

procedure TFrmMain.Button5Click(Sender: TObject); //����ܰ�ť5
var
  f: TEnterDlg;
begin
  index:=4;
  f := TEnterDlg.Create(Self);  //�����������ʾ
  f.ShowModal;

end;

procedure TFrmMain.Button6Click(Sender: TObject); //����ܰ�ť6
var
  f: TEnterDlg;
begin
  index:=5;
  f := TEnterDlg.Create(Self);  //�����������ʾ
  f.ShowModal;
end;

procedure TFrmMain.Button7Click(Sender: TObject);  //����ܰ�ť7
var
  f: TEnterDlg;
begin
  index:=6;
  f := TEnterDlg.Create(Self);  //�����������ʾ
  f.ShowModal;
end;

procedure TFrmMain.Button8Click(Sender: TObject); //����ܰ�ť8
var
  f: TEnterDlg;
begin
  index:=7;
  f := TEnterDlg.Create(Self);  //�����������ʾ
  f.ShowModal;

end;

procedure TFrmMain.Button9Click(Sender: TObject);
var
  f: TEnterDlg;
begin
  index:=-1;                    //ʶ�����ť���
  f := TEnterDlg.Create(Self);  //�����������ʾ
  f.ShowModal;
end;

//ģ��������а��������¼�
//ͬ��������ʾ,��������ʱ��ʾ��,������ʾ����
procedure TFrmMain.FormKeyDown(Sender: TObject; var Key: Word;
                                           Shift: TShiftState);
begin
    If Key = 1 Then                // ��������,���������ͷ�ԭ�����ٶ�ȡ�°���ֵ
        begin
        mSyncKeyboard($FF);        // �ͷ��ϴεİ���
        mSyncKeyboard(mKeyCode);   //���ݼ�ֵ��ͬ����ʾ
        end;

end;

//ģ��������а��������¼�
//ͬ��������ʾ,��������ʱ��ʾ��,������ʾ����
procedure TFrmMain.FormKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
   If Key = 1 Then
      mSyncKeyboard($FF);   // ���̰����ɿ�
end;


end.





