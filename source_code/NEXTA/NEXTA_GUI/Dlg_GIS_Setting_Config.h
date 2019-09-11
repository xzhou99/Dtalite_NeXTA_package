#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"


// CDlg_GIS_Setting_Config dialog

class CDlg_GIS_Setting_Config : public CDialog
{
	DECLARE_DYNAMIC(CDlg_GIS_Setting_Config)

public:
	CString message_str;

	CTLiteDoc* m_pDoc;

	CDlg_GIS_Setting_Config(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_GIS_Setting_Config();

	std::vector<CString> m_LinkFieldStringVector;
// Dialog Data
	enum { IDD = IDD_DIALOG_GIS_IMPORTING_GUIDE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonLoadLinkFile();
	CString m_LinkShapeFileString;
	afx_msg void OnCbnSelchangeComboLinkid();
	afx_msg void OnCbnSelchangeComboLinknamefield();
	afx_msg void OnCbnSelchangeComboDirection();
	afx_msg void OnCbnSelchangeComboDirection2();
	afx_msg void OnCbnSelchangeComboLengthUnit();
	afx_msg void OnCbnSelchangeComboLength();
	CComboBox m_LinkIDList;
	CComboBox m_LinkNameList;
	CComboBox m_TMCList;
	CComboBox m_SensorIDList;
	CComboBox m_LinkDirection;
	CComboBox m_LengthList;
	CComboBox m_FromNodeList;
	CComboBox m_ToNodeList;
	CComboBox m_LinkDirectionOption;
	CComboBox m_UnitLengthOption;
	afx_msg void OnBnClickedCheckAutoGenerateFromToNodes();
	BOOL m_bGenerateFromToNodes;
	CComboBox m_RNumberOfLanes;
	CComboBox m_RSpeedLimit;
	CComboBox m_RCapacity;
	CComboBox m_RTMC;
	CComboBox m_RSensorID;


	CString GetStringFromComboBox(CComboBox* pBox)
	{
	CString Text,str;

	if(pBox->GetLBTextLen (pBox->GetCurSel ())>=1)
	 pBox->GetLBText( pBox->GetCurSel (),Text);
	
	
	return Text;
	}
			

	afx_msg void OnBnClickedCheckRFields();
	BOOL m_bRFields;
	afx_msg void OnCbnSelchangeComboModeType();
	BOOL m_bManageLanes;
	afx_msg void OnBnClickedCheckAutoGenerateFromToNodes3();
	CComboBox m_NumberOfLanes;
	CComboBox m_SpeedLimit;
	CComboBox m_Capacity;
	CComboBox m_LaneVSLinkCapacity;
	CComboBox m_OneWayLanes;
	afx_msg void OnCbnSelchangeComboOnewayTwoway();
	CComboBox m_LinkType;
	CComboBox m_ModeType;
	afx_msg void OnBnClickedReservedfields();
	CEdit m_CentroidFile;
	CEdit m_ConnectorFile;
	CComboBox m_CentroidTAZ;
	CComboBox m_CentroidNodeID;
	CComboBox m_ConnectorZoneEnd;
	CComboBox m_ConnectorNodeEnd;
	afx_msg void OnBnClickedCheckCentroidConnector();
	BOOL m_bCentroidConnector;
	BOOL m_bWithSensorData;
	afx_msg void OnBnClickedCheckWithSensorData();
	CComboBox m_NodeID;
	CComboBox m_NodeName;
	CComboBox m_NodeTAZ;
	CEdit m_TAZThreshold;
	afx_msg void OnBnClickedButtonLoadNodeFile2();
	CString m_NodeShapeFileString;
	afx_msg void OnBnClickedButtonLoadZoneFile();
	CString m_ZoneShapeFileString;
	CComboBox m_ZoneTAZ;
	CComboBox m_ZoneNodeID;
	afx_msg void OnBnClickedButtonLoadConnectorFile();
	afx_msg void OnBnClickedButtonExternalexcel();
	CComboBox m_LinkTYpeDataList;
	afx_msg void OnCbnSelchangeComboLinksensorId();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonLoadLinkFile2();
	afx_msg void OnBnClickedButtonLoadLinkTypeReload();
	CComboBox m_LinkTypeCombo;
	afx_msg void OnBnClickedButtonLoadLinkFile3();
	afx_msg void OnBnClickedButtonLoadLinkFileTmcSpeedFile();
	afx_msg void OnBnClickedButtonLoadLinkFileSensorDataFile();
	BOOL m_DemandData;
	afx_msg void OnBnClickedCheckWithDemandData();
	afx_msg void OnBnClickedButtonLoadLinkTypeHelp();
	afx_msg void OnBnClickedButtonLoadSensorDataHelp();
	afx_msg void OnBnClickedButtonLoadLinkShapeHelp();
	afx_msg void OnBnClickedButtonLoadLinkSampleFile();
	afx_msg void OnBnClickedButtonLoadDemandDataHelp();
	afx_msg void OnBnClickedButtonLoadGisSettingDataHelp();
	afx_msg void OnBnClickedButtonLoadCentroidFile();
	afx_msg void OnStnClickedStaticC3();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedButtonEditDemandMetaDataBase();
	afx_msg void OnCbnSelchangeComboNodeid();
	afx_msg void OnCbnSelchangeComboNodenamefield();
};
