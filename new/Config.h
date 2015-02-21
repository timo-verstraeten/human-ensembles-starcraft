#pragma once
#ifndef INC_CONFIG_H
#define INC_CONFIG_H

#include <string>
#include <Windows.h>
#include <tchar.h>


class Config
{
public:
    Config();
    virtual ~Config() {};

    virtual double getAlpha();
    virtual double getLambda();
    virtual unsigned int getNumEpisodes();
    virtual double getShapingWeight();

    virtual std::string getExperimentName();
    virtual unsigned int getNumTrials();

    virtual std::string getOutputPath();
    virtual bool getEnableLogging();

private:
    LPCTSTR m_filename;

};

#endif // INC_CONFIG_H
