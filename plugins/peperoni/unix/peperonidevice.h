/*
  Q Light Controller
  peperonidevice.h

  Copyright (c) Heikki Junnila

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef PEPERONIDEVICE_H
#define PEPERONIDEVICE_H

#include <QThread>

struct usb_dev_handle;
struct usb_device;
class QString;
class QByteArray;
class Peperoni;

class PeperoniDevice : public QThread
{
    Q_OBJECT

    /********************************************************************
     * Initialization
     ********************************************************************/
public:
    PeperoniDevice(Peperoni *parent, struct usb_device* device, quint32 line);
    virtual ~PeperoniDevice();

    /** Find out, whether the given USB device is a Peperoni device */
    static bool isPeperoniDevice(const struct usb_device* device);

    /** Returns the number of output universes this device supports */
    static int outputsNumber(const struct usb_device* device);

    /********************************************************************
     * Device information
     ********************************************************************/
public:
    QString name(quint32 line) const;
    QString infoText(quint32 line) const;

protected:
    void extractName();

protected:
    QString m_name;
    quint32 m_line;

    /********************************************************************
     * Open & close
     ********************************************************************/
public:
    /** Interface mode specification */
    enum OperatingMode
    {
        CloseMode  = 1 << 0,
        OutputMode = 1 << 1,
        InputMode  = 1 << 2
    };

    void open(OperatingMode mode);
    void close(OperatingMode mode);

    const struct usb_device* device() const;
    const usb_dev_handle* handle() const;

    /** The device current open mode */
    int m_operatingMode;

protected:
    struct usb_device* m_device;
    usb_dev_handle* m_handle;
    int m_firmwareVersion;
    int m_blockingControlWrite;
    QByteArray m_bulkBuffer;

    /********************************************************************
     * Input worker thread
     ********************************************************************/
protected:
    bool m_running;

    /** Last universe data that has been received */
    QByteArray m_dmxInputBuffer;

private:
    /** @reimp */
    void run();

signals:
    /**
     * Signal that is emitted when an input channel's value is changed
     *
     * @param universe The universe where the event happened
     * @param line The input line that received the signal
     * @param channel The channel whose value has changed
     * @param value The changed value
     */
    void valueChanged(quint32 universe, quint32 line, quint32 channel, uchar value);

    /********************************************************************
     * Write
     ********************************************************************/
public:
    void outputDMX(quint32 line, const QByteArray& universe);
};

#endif
