#include "pasword.h"

bool PasWord::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type()) {
        case QEvent::Enter:
            PassEdit->setEchoMode(QLineEdit::Normal);
            break;
        case QEvent::Leave:
            PassEdit->setEchoMode(QLineEdit::Password);
            break;
        default:
            break;
    }
    return QWidget::eventFilter(watched, event);
}

PasWord::PasWord(QWidget *parent) : QWidget(parent)
{
    QLabel *pngLabel = new QLabel;
    pngLabel->setAlignment(Qt::AlignCenter);
    pngLabel->setPixmap(QPixmap(":/Green/images/eye.png").scaled(20, 20));
    QFont font = QFont("FZZJ-QDXKJW", 18);
    font.setBold (true);
    PassEdit = new QLineEdit;
    PassEdit->setObjectName("PassEdit");
    PassEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    PassEdit->setEchoMode(QLineEdit::Password);
    pngLabel->installEventFilter(this);
    PassEdit->setFont(font);
    QCursor *myCursor = new QCursor(QPixmap(":/Green/images/Arrow.png"));
    this->setCursor (*myCursor);
    QFrame *frame = new QFrame;
    frame->setObjectName("Pframe");
    QStringList LineSet;
    LineSet.append(QString("QFrame#PframePassword"
                           "{"
                           "border:1px solid black;"
                           "}"));
    LineSet.append(QString("QLabel{"
                           "min-width:15px;"
                           "background-color:rgba(240,248,255,50%);"
                           "}"));
    LineSet.append(QString("QPassEdit{"
                           "background-color:rgba(240,248,255,50%);"
                           "border:none;"
                           "border: 1px solid black;"
                           "border-radius: 8px;"
                           "}"));
    frame->setStyleSheet(LineSet.join(""));
    QHBoxLayout *hlayout = new QHBoxLayout(frame);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);
    hlayout->addWidget(PassEdit);
    hlayout->addWidget(pngLabel);
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    vLayout->addWidget(frame);
}

QString PasWord::text() const
{
    return PassEdit->text();
}
