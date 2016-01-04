// findDlg.cpp : implementation file
//

#include "stdafx.h"
#include "find.h"
#include "findDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString strDirectoryPath;
int ClassNumber=0;	

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindDlg dialog

CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
: CDialog(CFindDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
//{{AFX_MSG_MAP(CFindDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON1, OnFilePath)
ON_BN_CLICKED(IDOK, OnAnalysis)
	ON_BN_CLICKED(IDC_BUTTON2, OnClassDiaoyong)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindDlg message handlers

BOOL CFindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFindDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFindDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFindDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFindDlg::OnFilePath() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	BROWSEINFO bi;	
	char name[MAX_PATH];	
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();	
	bi.pszDisplayName = name;	
	bi.lpszTitle = "选择文件夹目录";	
	bi.ulFlags = BIF_RETURNFSANCESTORS;	
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);	
	if(idl == NULL)		return ;	
	SHGetPathFromIDList(idl, strDirectoryPath.GetBuffer(MAX_PATH));	
	strDirectoryPath.ReleaseBuffer();	
	if(strDirectoryPath.IsEmpty())		
		return;	
	if(strDirectoryPath.Right(1)!="\\")		
		strDirectoryPath+="\\";	
	
	GetDlgItem(IDC_STATIC)->SetWindowText(strDirectoryPath);
	
	UpdateData(FALSE);
}

struct StoreClass
{
	CString StoreClassName;
	int ObjectClassNum;
	int ClassMethodNum;
	bool BE;
}ClassName[1000];

void CFindDlg::OnAnalysis() 
{
	// TODO: Add your control notification handler code here
	//输出文件路径
	CString OutPutPath;	
	::GetModuleFileName(NULL, OutPutPath.GetBuffer(_MAX_PATH), _MAX_PATH);
	OutPutPath.ReleaseBuffer();
	OutPutPath = OutPutPath.Left(OutPutPath.ReverseFind('\\'));
	OutPutPath += "\\output.txt";
	CStdioFile OutPutfile;
	CString strbuf;//缓存零时数据
	BOOL ExistClass=FALSE;//判断本文件中是否含有类,初试状态设置为无
	CString StoreClassName[500];//用来存储类名
	CString ObjectClass,strObjCdata,ClassMethodName[1000];//用来存储属性名,方法名
	int ObjectClassNum=0,ClassMethodNum=0;//类个数，属性个数,方法个数
	CFileFind find;
	CStdioFile sFile;
	CString ClassFilename,FilePath,strReadData,FileName;
	ClassFilename+=strDirectoryPath;
	ClassFilename+="*.h";
	BOOL bfind=find.FindFile(ClassFilename);
	while(bfind)
	{
		bfind=find.FindNextFile();
		FileName=find.GetFileName();
		FilePath=strDirectoryPath;
		FilePath+=FileName;
		if (!sFile.Open(FilePath, CStdioFile::modeRead))
		{
			return;
		}
		
		while(1)
		{
			sFile.ReadString(strReadData);
			strbuf=strReadData;//零时缓存读取的数据
			if (strReadData=="")
			{
				sFile.ReadString(strReadData);
				if (strReadData=="")
				{
					sFile.ReadString(strReadData);
					if (strReadData=="")
					{
						sFile.ReadString(strReadData);
						if (strReadData=="")
						{
							break;
						}
					}
				}
			}
			
			//统计多少个类，类名
			if (strstr(strReadData,"class")&&strReadData.GetAt(0)=='c')
			{
				if (strReadData.Right(1)==";")
				{
					continue;
				}
				ExistClass=TRUE;
				ClassNumber++;
				AfxExtractSubString(ClassName[ClassNumber].StoreClassName,strReadData,1,' ');
			}
			
			if(ExistClass)
			{
				//统计多少个属性
				strReadData.Remove(' ');
				strReadData.Remove('	');
				strObjCdata=strReadData.Left(6);
				if (strstr(strObjCdata,"int")||strstr(strObjCdata,"string")||strstr(strObjCdata,"size_t"))
				{
					if (strstr(strReadData,"="))
					{
						ObjectClassNum++;
				    	continue;
					}
					else if (strReadData.Right(2)==");")
					{
						goto fangfa;
					}
					ObjectClassNum++;
					continue;
				}
				
				//统计多少个方法,并记录方法名称
fangfa: 		if (strReadData.Right(2)==");")
				{
					ClassMethodNum++;
					CString strReData;
					AfxExtractSubString(strReData, strbuf,0, ' ');
					ClassMethodName[ClassMethodNum]=strbuf;
					ClassMethodName[ClassMethodNum].Replace(strReData,NULL);
					ClassMethodName[ClassMethodNum].Remove(';');
					if (ClassMethodName[ClassMethodNum]=="")//去除类套用的记次
					{
						ClassMethodNum--;
						continue;
					}
					ClassMethodName[ClassMethodNum]=ClassMethodName[ClassMethodNum].Mid(1,ClassMethodName[ClassMethodNum].GetLength());
				}
				
				//统计方法的行数
				if (ClassMethodNum!=0&&strbuf=="};")
				{
					int MethodLines[100]={0};
					CString MethodPath,strdata,Cppmethdname="";
					MethodPath=FilePath;
					MethodPath.Replace(".h",".cpp");
					CStdioFile CppFile;
					for (int i=1;i<=ClassMethodNum;i++)
					{
						Cppmethdname="::";
						Cppmethdname+=ClassMethodName[i];//ClassMethodNum
						if (!CppFile.Open(MethodPath, CStdioFile::modeRead))
						{
							break;
						}
						while (1)
						{
							CppFile.ReadString(strdata);
							if (strdata=="")//连续三行是空则跳出本文件
							{
								CppFile.ReadString(strdata);
								if (strdata=="")
								{
									CppFile.ReadString(strdata);
									if (strdata=="")
									{
										break;
									}
								}
							}
							if (strstr(strdata,Cppmethdname))
							{
                       loop:	CppFile.ReadString(strdata);
	                        	if (strdata==""||strdata.Left(3)=="	//")//去除空行和注释
								{
	                        		goto loop;
								}
								if (strdata=="}")
								{
									break;
								}

	                         	MethodLines[i]++;//统计方法的行数ClassMethodNum
								goto loop;
							}
						}
						CppFile.Close();
					}
					//求最大行数和最小行数
					int minLines=MethodLines[1],maxLines=MethodLines[1];
					for (int j=1;j<=ClassMethodNum;j++)
					{
						if (MethodLines[j]>maxLines) maxLines=MethodLines[j];
						if (MethodLines[j]<minLines&&MethodLines[j]!=0) minLines=MethodLines[j];
					}
					//输出当前类的名字和他的多少个属性、多少个方法，最长和最短的方法有多少行源代码

					if (!OutPutfile.Open(OutPutPath,CStdioFile::modeReadWrite))
					{
						MessageBox("输出错误！");
						return;
					}
					//输出类名
					OutPutfile.SeekToEnd();
					CString NumData,strInputData;
					OutPutfile.WriteString("                       \n");//fangzhifugai
					strInputData="类名：";
					strInputData+=ClassName[ClassNumber].StoreClassName;
					strInputData+="\n";
					OutPutfile.WriteString(strInputData);

					strInputData="属性个数：";
					NumData.Format("%d",ObjectClassNum);
					strInputData+=NumData;
					strInputData+="\n";
					OutPutfile.WriteString(strInputData);

					strInputData="方法个数：";
					NumData.Format("%d",ClassMethodNum);
					strInputData+=NumData;
					strInputData+="\n";
					OutPutfile.WriteString(strInputData);

					strInputData="当前类中方法最短行数：";
					NumData.Format("%d",minLines);
					strInputData+=NumData;
					strInputData+="\n";
					OutPutfile.WriteString(strInputData);

					strInputData="当前类中方法最长行数：";
					NumData.Format("%d",maxLines);
					strInputData+=NumData;
					strInputData+="\n";
					OutPutfile.WriteString(strInputData);

					OutPutfile.Close();
					ObjectClassNum=0,ClassMethodNum=0;//置0

				}
				
			}
		}
		
		
		
		
		ExistClass=FALSE;
		sFile.Close();
	}
	if (!OutPutfile.Open(OutPutPath,CStdioFile::modeReadWrite))
	{
		MessageBox("打开输出文件失败！");
		return;
	}
	CString NumData,strInputData;
	strInputData="类的个数：";
	NumData.Format("%d",ClassNumber);
	strInputData+=NumData;
	strInputData+="\n";
	OutPutfile.WriteString(strInputData);
	OutPutfile.Close();


	CString num;
	num.Format("%d",ClassNumber);
	MessageBox(num);
	
}

void CFindDlg::OnClassDiaoyong() 
{
	// TODO: Add your control notification handler code here
	//输出文件路径
	CString OutPutPath;	
	::GetModuleFileName(NULL, OutPutPath.GetBuffer(_MAX_PATH), _MAX_PATH);
	OutPutPath.ReleaseBuffer();
	OutPutPath = OutPutPath.Left(OutPutPath.ReverseFind('\\'));
	OutPutPath += "\\output.txt";
	CStdioFile OutPutfile;
	CString strbuf,strNameData;//缓存零时数据
	int num=0;
	BOOL ExistClass=FALSE;//判断本文件中是否含有类,初试状态设置为无
	CFileFind find;
	CStdioFile sFile;
	CString ClassFilename,FilePath,strReadData,FileName;
	ClassFilename+=strDirectoryPath;
	ClassFilename+="*.h";
	BOOL bfind=find.FindFile(ClassFilename);
	while(bfind)
	{
		bfind=find.FindNextFile();
		FileName=find.GetFileName();
		FilePath=strDirectoryPath;
		FilePath+=FileName;
		if (!sFile.Open(FilePath, CStdioFile::modeRead))
		{
			return;
		}

		for (int i=1;i<=ClassNumber;i++)
		{
			ClassName[i].BE=FALSE;
		}
		
		while(1)
		{
			sFile.ReadString(strReadData);
			if (strReadData=="")
			{
				sFile.ReadString(strReadData);
				if (strReadData=="")
				{
					sFile.ReadString(strReadData);
					if (strReadData=="")
					{
						break;
					}
				}
			}
			
			if (strstr(strReadData,"class")&&strReadData.GetAt(0)=='c')
			{
				if (strReadData.Right(1)==";")
				{
					continue;
				}
				AfxExtractSubString(strNameData,strReadData,1,' ');
				strbuf=strNameData;
				strbuf+="   含有的类:    ";
				ExistClass=TRUE;
				continue;
			}
			if (!(strReadData.Right(2)=="};")&&ExistClass)
			{
				//统计含有多少个类，类名
				for (num=1;num<=ClassNumber;num++)
				{
					if (strstr(strReadData,ClassName[num].StoreClassName)&&!(strstr(strReadData,strNameData)))
					{
						ClassName[num].BE=true;
					}
				}
			}

			if (strReadData.Right(2)=="};")
			{
				ExistClass=FALSE;				
				for (num=1;num<=ClassNumber;num++)
				{
					if (ClassName[num].BE)
					{
						strbuf+=ClassName[num].StoreClassName;
						strbuf+="  ";
					}
				}

				strbuf+="\n";
				if (!OutPutfile.Open(OutPutPath,CStdioFile::modeReadWrite))
				{
					MessageBox("输出文件错误!");
					return;
				}
				OutPutfile.SeekToEnd();
				OutPutfile.WriteString(strbuf);
				OutPutfile.Close();
			}

		}
		sFile.Close();
	}

	MessageBox("OK!");
	
}
