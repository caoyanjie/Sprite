#ifndef TIMING_H
#define TIMING_H

#include <QGroupBox>

class QLabel;
class QSpinBox;
class QRadioButton;
class QPushButton;
class QButtonGroup;

class Timing : public QGroupBox
{
    Q_OBJECT
public:
    explicit Timing(int x, int y, int w, int h, QWidget *parent = 0);

    QRadioButton *rbn_countDown;
    QRadioButton *rbn_timing;
    QRadioButton *rbn_playStop;
    QRadioButton *rbn_windowClose;
    QRadioButton *rbn_shutdown;

    QButtonGroup *groupButton1;
    QButtonGroup *groupButton2;

    int get_spbHour_value();
    int get_spbMinite_value();
    int get_spbSecond_value();
    int get_timingTarget();

protected:

private:
    QSpinBox *spb_hour;
    QLabel *lab_hour;
    QSpinBox *spb_minite;
    QLabel *lab_minite;
    QSpinBox *spb_second;
    QLabel *lab_second;
    QPushButton *pbn_ok;
    QPushButton *pbn_cancle;

    QLabel *lab_bg1;
    QLabel *lab_bg2;
    QLabel *lab_bg3;

signals:
    void timing_pbnOk_click();
    void timing_pbnCancle_click();

public slots:

};

#endif // TIMING_H
