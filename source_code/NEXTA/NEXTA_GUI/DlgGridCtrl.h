#pragma once

#include "TLiteDoc.h"
#include "GridCtrl_src\\GridCtrl.h"
#include "CGridListCtrlEx\\CGridListCtrlEx.h"
#include <vector>
#include "afxwin.h"

using std::map;
class ZoneRecord
{
public:
	ZoneRecord(int z, int n):zone_num(z),node_name(n){};
	int zone_num;
	int node_name;
};

// CDlgODDemandGridCtrl dialog

class CDlgODDemandGridCtrl : public CDialog
{
	DECLARE_DYNAMIC(CDlgODDemandGridCtrl)

public:
	bool m_bLoadDemandForLargeScaleNetwork;
	CDlgODDemandGridCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgODDemandGridCtrl();

// Dialog Data
	enum { IDD = IDD_ODDEMAND_GRID_DIALOG };

	std::vector<ZoneRecord> ZoneRecordSet;
	CTLiteDoc* m_pDoc;
	void DisplayDemandMatrix();
	void DisplayDemandTypeTable();

	int m_SelectedDemandMetaType;
	int m_DemandSequenceNo;

	std::map<CString,float> m_ODMatrixMap;

	CString GetODKey(int origin,int destination)
	{
	CString str;
	str.Format("%d,%d",origin,destination);
	return str;

	}


	
	float GetODValue(int origin,int destination)
	{
	
		if (m_ODMatrixMap.find(GetODKey(origin, destination))!= m_ODMatrixMap.end())
		{
			return m_ODMatrixMap[GetODKey(origin, destination)];
		}else
		{
			return 0;
		}

	}



	void SetODMatrx(int origin,int destination,float value)
	{
	m_ODMatrixMap[GetODKey(origin,destination)] += value;

	CString str;
	str.Format("o%d",origin);
	m_ODMatrixMap[str] +=value;

	str.Format("d%d",destination);
	m_ODMatrixMap[str] +=value;
	str.Format("total",destination);
	m_ODMatrixMap[str] +=value;

	}

	CString m_SelectedFileName;
	std::vector<CString> DemandFileNameVector;
	void LoadDemandMatrixFromDemandFile(int DemandFileSequenceNo, int SelectedDemandMetaType);
private:
	CGridCtrl m_ODMatrixGrid;
	CListCtrl m_DemandTypeGrid;
	CListCtrl m_DemandFileGrid;
	bool m_bSizeChanged;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGridSavequit();
	afx_msg void OnBnClickedGridQuit();

	void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	bool SaveDemandMatrix();

	bool SaveZoneCSVFileExt(LPCTSTR lpszFileName);
	afx_msg void OnBnClickedButtonCreatezones();
	afx_msg void OnBnClickedButtonEditZoneNodeMapping();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedGridSavequit2();
	afx_msg void OnLvnItemchangedDemandtypelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedEditMetaDatabase();
	afx_msg void OnLvnItemchangedDemandtypelist2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedEditMetaDatabase2();
	afx_msg void OnBnClickedEditVehicleTypeFile();
	afx_msg void OnBnClickedEditVehicleEmissionsFile();
	afx_msg void OnBnClickedEditMetaDatabase3();
	afx_msg void OnHdnItemdblclickDemandtypelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnLinkClickedDemandtypelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonReload();
	afx_msg void OnBnClickedButtonExportMatrix();
	afx_msg void OnBnClickedButtonExportColumn();
	afx_msg void OnBnClickedEditMetaDatabase4();
	afx_msg void OnLvnItemchangedDemandfilelist(NMHDR *pNMHDR, LRESULT *pResult);
};
