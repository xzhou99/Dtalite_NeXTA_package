#pragma once

#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "TLiteView.h"
#ifndef _WIN64
#include "Data-Interface\\include\\ogrsf_frmts.h"
#endif
#include "afxwin.h"
// CDlg_GISDataExchange dialog


class CDlg_GISDataExchange : public CDialog
{
	DECLARE_DYNAMIC(CDlg_GISDataExchange)

public:

	_GIS_DATA_TYPE m_GIS_data_type;
	CDlg_GISDataExchange(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_GISDataExchange();

	CTLiteDoc* m_pDoc;
// Dialog Data
	enum { IDD = IDD_DIALOG_GIS_IMPORT_SHAPE_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	void SaveTNPProject();
	void ExportToGISFile(LPCTSTR lpszCSVFileName,LPCTSTR lpszShapeFileName, CString GISTypeString);
	void ExportDataToCSV(CString string);
	void ExportLinkLayerToShapeFile(CString string);


	afx_msg void OnBnClickedImportGpsShapeFile();
	afx_msg void OnBnClickedButtonFindSensorFile();
	CString m_GIS_ShapeFile;
	afx_msg void OnLbnSelchangeList1();
	CListBox m_MessageList;
	afx_msg void OnBnClickedExportCsvFile();
	afx_msg void OnBnClickedExportGpsShapeFile();
	afx_msg void OnBnClickedExportKmlFile();
	afx_msg void OnBnClickedButtonViewSampleFile();
	afx_msg void OnBnClickedButtonLoadSampleShapeFile();
	afx_msg void OnBnClickedButtonImportGisShapeFile();
	afx_msg void OnBnClickedButtonExporttoCsvFile();
	afx_msg void OnBnClickedButtonViewExportedKmlFile();
	afx_msg void OnBnClickedButtonFindCsfFile();
	CString m_CSV_File;
	afx_msg void OnBnClickedExportShapeFile();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedExportGisShapeFile();
	CListBox m_GISDataType_List;
	virtual BOOL OnInitDialog();

	BOOL m_bCreateNodeFromLink;
	BOOL m_SaveInputNodeLinkFiles;
	afx_msg void OnBnClickedImportGpsOsmPointShapeFile();
	afx_msg void OnBnClickedImportGpsOsmPointShapeFile2();
};
