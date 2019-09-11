#pragma once
#include "DlgScenarioTab.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "CXMLFile_SRC\\XMLFile.h"
#include "CSVParser.h"

enum SCENARIO_TYPE { _WORKZONE = 0, _DYNMSGSIGN, _LINKBASEDTOLL, _GENERICTRAFFICCONTROL, _INCIDENT};
// CDlgScenario dialog

class CDlgScenario : public CDialog
{
	DECLARE_DYNAMIC(CDlgScenario)

public:
	CDlgScenario(int idx=0,CWnd* pParent = NULL );
	virtual ~CDlgScenario();
	CTLiteDoc* m_pDoc;
	

// Dialog Data
	enum { IDD = IDD_DIALOG_SCENARIO };
private:
	CTabCtrl m_TabCtrl;
	CDlgScenarioTab* p_SubTabs[6];
	CXMLFile m_XMLFile;
	int m_PrevTab;
public:
	int m_SelectTab;

	BOOL ReadXMLFile(const char* ElementName, std::vector<std::string>& name_vector,std::vector<std::vector<std::string>>& value_vector);
	BOOL ReadScenarioCSVFile(const char* ElementName, std::vector<std::string>& name_vector,std::vector<std::vector<std::string>>& value_vector);
	BOOL ReadCapacityReductionScenario(std::vector<std::vector<std::string>>& value_vector);

	void GetDefaultInfo(int i, std::vector<std::string>& HeaderList, std::vector<CString>& DefaultList);
	void SetRectangle();
	std::vector<std::string> GetLinkString();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTcnSelchangeScenarioTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonAddScenario();
	afx_msg void OnBnClickedButtonEditDataInExcel();
};
