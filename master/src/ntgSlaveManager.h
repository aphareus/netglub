/******************************************************************************
**
** Copyright (C) 2009-2010 DIATEAM. All rights reserved.
**
** This file is part of the "NetGlub" program.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
******************************************************************************/

#ifndef NTGSLAVEMANAGER_H
#define NTGSLAVEMANAGER_H

#include <QTcpServer>
#include <QSslConfiguration>
#include <ntgTransformManager.h>

class NtgSlaveProxy;
class NtgRpcSslTransport;
class NtgSlaveCreds;

class NtgSlaveManager : public QTcpServer
{
  Q_OBJECT
  public:
    NtgSlaveManager(const QHostAddress & ip, quint16 port, const QSslConfiguration & tlsConf, NtgTransformManager * transformManager);

    NtgSlaveProxy * getSlaveProxy(QString proxyName) const;
    QList<NtgSlaveProxy*> getSlavesWithTransformType(const QString & transformName) const;
    QStringList availableTransforms() const;
    QHash<QString,NtgTransformModel> allSlaveTransfomsModels() const;
    bool isAvailableTransformForUser(const QString & name);

  signals:
    void newSlave(QString slaveProxyName);
    void slaveGone(QString slaveProxyName);

  protected:
    virtual void incomingConnection(int socketDescriptor);
    bool _validateSlaveCreds(NtgSlaveCreds * creds);
    NtgTransformManager * _transformManager;

  protected slots:
    void _removeSlave();

  private:

    struct SlaveInfo
    {
      SlaveInfo();
      ~SlaveInfo();

      NtgRpcSslTransport * transport;
      NtgSlaveProxy * proxy;
      NtgSlaveCreds * creds;
    };

    QHash<QString, SlaveInfo*> _slaveInfos;
    QSslConfiguration _defaultConf;
};

#endif // NTGSLAVEMANAGER_H