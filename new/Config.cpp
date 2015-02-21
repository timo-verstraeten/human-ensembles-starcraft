#include "Config.h"

Config::Config()
{
    m_filename =  _T(".\\config.ini");
}


double Config::getAlpha()
{
    TCHAR buffer[260];
    GetPrivateProfileString(_T("TRIAL"), _T("alpha"), _T("0.4"), buffer, 260, m_filename);

    return _tstof(buffer);
}

double Config::getLambda()
{
    TCHAR buffer[260];
    GetPrivateProfileString(_T("TRIAL"), _T("lambda"), _T("0.9"), buffer, 260, m_filename);

    return _tstof(buffer);
}

unsigned int Config::getNumEpisodes()
{
    TCHAR buffer[260];
    GetPrivateProfileString(_T("TRIAL"), _T("numEpisodes"), _T("10"), buffer, 260, m_filename);

    return _ttoi(buffer);
}

double Config::getShapingWeight()
{
    TCHAR buffer[260];
    GetPrivateProfileString(_T("TRIAL"), _T("shapingWeight"), _T("1.0"), buffer, 260, m_filename);

    return _tstof(buffer);
}

std::string Config::getExperimentName()
{

    TCHAR buffer[260];
    GetPrivateProfileString(_T("EXPERIMENT"), _T("name"), _T("Trials"), buffer, 260, m_filename);

    std::wstring arr_w(buffer);
    std::string arr_s(arr_w.begin(), arr_w.end());
    return arr_s;
}

unsigned int Config::getNumTrials()
{
    TCHAR buffer[260];
    GetPrivateProfileString(_T("EXPERIMENT"), _T("numTrials"), _T("1"), buffer, 260, m_filename);

    return _ttoi(buffer);
}

std::string Config::getOutputPath()
{
    TCHAR buffer[260];

    GetPrivateProfileString(_T("GENERAL"), _T("outputPath"), _T("C:\\"), buffer, 260, m_filename);

    std::wstring arr_w(buffer);
    std::string arr_s(arr_w.begin(), arr_w.end());
    return arr_s;
}

bool Config::getEnableLogging()
{
    TCHAR buffer[260];

    GetPrivateProfileString(_T("GENERAL"), _T("enableLogging"), _T("0"), buffer, 260, m_filename);

    return (_ttoi(buffer) == 1);
}