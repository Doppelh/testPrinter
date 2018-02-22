#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonPrint_clicked()
{
    QPrinter *myPrinter = new QPrinter();

    this->on_Print(myPrinter);
}

void MainWindow::on_pushButtonPreview_clicked()
{
    QPrinter myPrinter(QPrinter::HighResolution);
    myPrinter.setPrinterName("LBP251");

    QPrintPreviewDialog myPreview(&myPrinter, this);

    connect(&myPreview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(on_Print(QPrinter*)));
    myPreview.exec();
}

void MainWindow::on_Print(QPrinter *arPrinter)
{
    ui->textEdit->append("on_Print 실행중..");
    QPrintDialog dialog(arPrinter, this);

    ui->textEdit->append("dialog 실행중..");

    if(dialog.exec() == QDialog::Rejected)
    {
        ui->textEdit->append(QString(arPrinter->isValid()));
        qDebug()<<arPrinter->isValid();
        return;
    }

    ui->textEdit->append("dialog 실행생성");

    QPainter painter;
    painter.begin(arPrinter);

    QFont font = ui->fontComboBox->currentFont();

    font.setBold(ui->checkBoxBold->isChecked());
    font.setPointSize(ui->lineEditFontSize->text().toInt());
    painter.setFont(font);

    ui->textEdit->append("프린팅..");


    QStringList numberList, hanList, engList;

    for(int i = 0; i < 500 ; i++)
    {
        numberList<<QString::number(i);
        hanList<<"가나다";
        engList<<"ABC";
    }
    int leftRightLength, UpDownLength, xScale, yScale, leftBlank, upBlank, letterSideLength;

    //x, y의 초기값은 위치정보 + 들여쓰기
    xScale = 0;
    yScale = 0;
    leftRightLength = ui->lineEditLeftRightLength->text().toInt();
    UpDownLength = ui->lineEditUpDownLength->text().toInt();
    leftBlank = ui->lineEditLeftBlank->text().toInt();
    upBlank = ui->lineEditUpBlank->text().toInt();
    letterSideLength = ui->lineEditFontSize->text().toInt();

    //x값에 왼쪽 여백만큼 더하기
    xScale = xScale + leftBlank;

    yScale = yScale + upBlank;

    //x값, y값, 왼쪽 이동값, 아래 이동값, 값 사이 간격(위 아래)
    for (int k = 0; k < 54; ++k)
    {
        painter.drawText(xScale, yScale, numberList.at(k));
        painter.drawText(xScale, yScale+letterSideLength, hanList.at(k));
        painter.drawText(xScale, yScale+letterSideLength*2, engList.at(k));

        //x좌표를 좌우사이간격(leftRightLength) 만큼 이동
        xScale = xScale + leftRightLength;

        //오른쪽 pageRect.width 초과시 줄바꿈
        if(arPrinter->pageRect().width() - leftBlank < xScale)
        {
            xScale = 0;
            xScale = xScale + leftBlank;

            yScale = yScale + UpDownLength;
        }

        //페이지 넘어갈 시 newPage
        if(arPrinter->pageRect().height() - upBlank < yScale)
        {
            xScale = 0;
            yScale = 0;

            xScale = xScale + leftBlank;

            yScale = yScale + upBlank;

            arPrinter->newPage();
        }
    }

    painter.end();

    QMessageBox::information(this, "COMPLETE", "Complete printing..", QMessageBox::Ok);
}
