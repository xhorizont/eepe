#include "mixerdialog.h"
#include "ui_mixerdialog.h"
#include "pers.h"
#include "helpers.h"

MixerDialog::MixerDialog(QWidget *parent, MixData *mixdata, int stickMode, QString * comment, int modelVersion, int eepromType, int delaySpeed) :
    QDialog(parent),
    ui(new Ui::MixerDialog)
{
    ui->setupUi(this);
    md = mixdata;
		mType = eepromType ;
		delaySlowSpeed = delaySpeed ;

    this->setWindowTitle(tr("DEST -> CH%1%2").arg(md->destCh/10).arg(md->destCh%10));
    populateSourceCB(ui->sourceCB, stickMode, 0, md->srcRaw, modelVersion);
    ui->sourceCB->addItem("3POS");
    ui->sourceCB->addItem("GV1 ");
    ui->sourceCB->addItem("GV2 ");
    ui->sourceCB->addItem("GV3 ");
    ui->sourceCB->addItem("GV4 ");
    ui->sourceCB->addItem("GV5 ");
    ui->sourceCB->addItem("GV6 ");
    ui->sourceCB->addItem("GV7 ");
    ui->sourceCB->setCurrentIndex(md->srcRaw);

    ui->sourceCB->removeItem(0);

    populateSpinGVarCB( ui->weightSB, ui->weightCB, ui->weightGvChkB, md->weight, -125, 125 ) ;
    populateSpinGVarCB( ui->offsetSB, ui->offsetCB, ui->offsetGvChkB, md->sOffset, -125, 125 ) ;

    ui->trimChkB->setChecked(md->carryTrim==0);
    ui->FMtrimChkB->setChecked(md->enableFmTrim);
    ui->lateOffsetChkB->setChecked(md->lateOffset);
    populateSwitchCB(ui->switchesCB,md->swtch, eepromType ) ;
    ui->warningCB->setCurrentIndex(md->mixWarn);
    ui->mltpxCB->setCurrentIndex(md->mltpx);
		ui->diffcurveCB->setCurrentIndex(md->differential) ;
		
		if (md->differential)
		{
			populateSpinGVarCB( ui->curvesSB, ui->curvesCB, ui->curveGvChkB, md->curve, -100, 100 ) ;
			ui->curveGvChkB->setVisible( true ) ;
		}
		else
		{
			populateCurvesCB(ui->curvesCB, md->curve ) ;
			ui->curvesCB->setVisible( true ) ;
			ui->curvesSB->setVisible( false ) ;
			ui->curveGvChkB->setVisible( false ) ;
      ui->curveGvChkB->setChecked( false ) ;
		}


		if ( delaySpeed )
		{
			ui->delayDownSB->setDecimals( 1 ) ;
			ui->delayDownSB->setSingleStep( 0.2 ) ;
			ui->delayDownSB->setMaximum ( 3.0 ) ;
			ui->delayDownSB->setValue(md->delayDown/5.0) ;
			
			ui->delayUpSB->setDecimals( 1 ) ;
			ui->delayUpSB->setSingleStep( 0.2 ) ;
			ui->delayUpSB->setMaximum ( 3.0 ) ;
    	ui->delayUpSB->setValue(md->delayUp/5.0) ;
			
			ui->slowDownSB->setDecimals( 1 ) ;
			ui->slowDownSB->setSingleStep( 0.2 ) ;
			ui->slowDownSB->setMaximum ( 3.0 ) ;
    	ui->slowDownSB->setValue(md->speedDown/5.0) ;
			
			ui->slowUpSB->setDecimals( 1 ) ;
			ui->slowUpSB->setSingleStep( 0.2 ) ;
			ui->slowUpSB->setMaximum ( 3.0 ) ;
    	ui->slowUpSB->setValue(md->speedUp/5.0) ;
		}
		else
		{
			ui->delayDownSB->setDecimals( 0 ) ;
			ui->delayDownSB->setSingleStep( 1 ) ;
			ui->delayDownSB->setMaximum ( 15 ) ;
			ui->delayUpSB->setDecimals( 0 ) ;
			ui->slowDownSB->setDecimals( 0 ) ;
			ui->slowUpSB->setDecimals( 0 ) ;
	    ui->delayDownSB->setValue(md->delayDown) ;
    	ui->delayUpSB->setValue(md->delayUp) ;
    	ui->slowDownSB->setValue(md->speedDown) ;
    	ui->slowUpSB->setValue(md->speedUp) ;
		}


    mixCommennt = comment;
    ui->mixerComment->setPlainText(mixCommennt->trimmed());

    valuesChanged();

    connect(ui->sourceCB,SIGNAL(currentIndexChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->weightSB,SIGNAL(valueChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->weightCB,SIGNAL(currentIndexChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->weightGvChkB,SIGNAL(stateChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->offsetSB,SIGNAL(valueChanged(int)),this,SLOT(valuesChanged()));
		connect(ui->offsetCB,SIGNAL(currentIndexChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->offsetGvChkB,SIGNAL(stateChanged(int)),this,SLOT(valuesChanged()));
		connect(ui->trimChkB,SIGNAL(toggled(bool)),this,SLOT(valuesChanged()));
    connect(ui->curvesCB,SIGNAL(currentIndexChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->curvesSB,SIGNAL(valueChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->curveGvChkB,SIGNAL(stateChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->diffcurveCB,SIGNAL(currentIndexChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->switchesCB,SIGNAL(currentIndexChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->warningCB,SIGNAL(currentIndexChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->mltpxCB,SIGNAL(currentIndexChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->delayDownSB,SIGNAL(valueChanged( double )),this,SLOT(valuesChanged()));
    connect(ui->delayUpSB,SIGNAL(valueChanged(double)),this,SLOT(valuesChanged()));
    connect(ui->slowDownSB,SIGNAL(valueChanged(double)),this,SLOT(valuesChanged()));
    connect(ui->slowUpSB,SIGNAL(valueChanged(double)),this,SLOT(valuesChanged()));
    connect(ui->FMtrimChkB,SIGNAL(stateChanged(int)),this,SLOT(valuesChanged()));
    connect(ui->mixerComment,SIGNAL(textChanged()),this,SLOT(valuesChanged()));
    connect(ui->lateOffsetChkB,SIGNAL(stateChanged(int)),this,SLOT(valuesChanged()));
}

MixerDialog::~MixerDialog()
{
    delete ui;
}

void MixerDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MixerDialog::valuesChanged()
{
	int oldcurvemode ;
	int limit = MAX_DRSWITCH ;
#ifndef SKY
  if ( mType )
	{
    limit += EXTRA_CSW ;
	}
#endif

    md->srcRaw       = ui->sourceCB->currentIndex()+1;
    md->weight       = numericSpinGvarValue( ui->weightSB, ui->weightCB, ui->weightGvChkB, md->weight, 100 ) ;
    md->sOffset      = numericSpinGvarValue( ui->offsetSB, ui->offsetCB, ui->offsetGvChkB, md->sOffset, 0 ) ;
    md->carryTrim    = ui->trimChkB->checkState() ? 0 : 1;
    md->swtch        = ui->switchesCB->currentIndex()-limit ;
    md->mixWarn      = ui->warningCB->currentIndex();
    md->mltpx        = ui->mltpxCB->currentIndex();
		if ( delaySlowSpeed )
		{
    	md->delayDown    = (ui->delayDownSB->value()+0.1 ) * 5 ;
    	md->delayUp      = (ui->delayUpSB->value()+0.1 ) * 5 ;
    	md->speedDown    = (ui->slowDownSB->value()+0.1 ) * 5 ;
    	md->speedUp      = (ui->slowUpSB->value()+0.1 ) * 5 ;
		}
		else
		{
    	md->delayDown    = ui->delayDownSB->value()+0.1 ;
    	md->delayUp      = ui->delayUpSB->value()+0.1 ;
    	md->speedDown    = ui->slowDownSB->value()+0.1 ;
    	md->speedUp      = ui->slowUpSB->value()+0.1 ;
		}

    md->enableFmTrim = ui->FMtrimChkB->checkState() ? 1 : 0;
    md->lateOffset   = ui->lateOffsetChkB->checkState() ? 1 : 0;
		
		oldcurvemode = md->differential ;
		md->differential = ui->diffcurveCB->currentIndex() ;

		if ( md->differential != oldcurvemode )
		{
			if (md->differential)
			{
				populateSpinGVarCB( ui->curvesSB, ui->curvesCB, ui->curveGvChkB, 0, -100, 100 ) ;
				ui->curveGvChkB->setVisible( true ) ;
			}
			else
			{
	      ui->curveGvChkB->setChecked( false ) ;
				populateCurvesCB(ui->curvesCB, 0 ) ;
				ui->curveGvChkB->setVisible( false ) ;
				ui->curvesSB->setVisible( false ) ;
				ui->curvesCB->setVisible( true ) ;
			}
	    md->curve = numericSpinGvarValue( ui->curvesSB, ui->curvesCB, ui->curveGvChkB, 0, 0 ) ;
		}
		else
		{
			if (md->differential)
			{
	  	 		md->curve = numericSpinGvarValue( ui->curvesSB, ui->curvesCB, ui->curveGvChkB, md->curve, 0 ) ;
			}
			else
			{
    		md->curve = ui->curvesCB->currentIndex()-16;
			}
		}


    if(ui->FMtrimChkB->checkState())
        ui->offset_label->setText("FmTrimVal");
    else
        ui->offset_label->setText("Offset");

    mixCommennt->clear();
    mixCommennt->append(ui->mixerComment->toPlainText());
}
