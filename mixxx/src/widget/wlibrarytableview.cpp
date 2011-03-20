// wlibrarytableview.cpp
// Created 10/19/2009 by RJ Ryan (rryan@mit.edu)

#include <QHeaderView>
#include <QPalette>
#include <QScrollBar>

#include "widget/wwidget.h"
#include "widget/wskincolor.h"
#include "widget/wlibrarytableview.h"
#include "../library/stardelegate.h"

WLibraryTableView::WLibraryTableView(QWidget* parent,
                                     ConfigObject<ConfigValue>* pConfig,
                                     ConfigKey vScrollBarPosKey)
        : QTableView(parent),
          m_pConfig(pConfig),
          m_vScrollBarPosKey(vScrollBarPosKey) {

    // Setup properties for table

    // Editing starts when clicking on an already selected item.
    setEditTriggers(QAbstractItemView::SelectedClicked);

    // This is to support rating of tracks
    setItemDelegate(new StarDelegate(this));

    //Enable selection by rows and extended selection (ctrl/shift click)
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setWordWrap(false);
    setShowGrid(false);
    setCornerButtonEnabled(false);
    setSortingEnabled(true);
    //Work around a Qt bug that lets you make your columns so wide you
    //can't reach the divider to make them small again.
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize(20);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setAlternatingRowColors(true);

    loadVScrollBarPosState();
}

WLibraryTableView::~WLibraryTableView() {
    qDebug() << "~WLibraryTableView";
    saveVScrollBarPosState();
}

void WLibraryTableView::setup(QDomNode node) {
}

void WLibraryTableView::loadVScrollBarPosState() {
    // TODO(rryan) I'm not sure I understand the value in saving the v-scrollbar
    // position across restarts of Mixxx. Now that we have different views for
    // each mode, the views should just maintain their scrollbar position when
    // you switch views. We should discuss this.
    m_iSavedVScrollBarPos = m_pConfig->getValueString(m_vScrollBarPosKey).toInt();
}

void WLibraryTableView::restoreVScrollBarPos() {
    //Restore the scrollbar's position (scroll to that spot)
    //when the search has been cleared
    verticalScrollBar()->setValue(m_iSavedVScrollBarPos);
}

void WLibraryTableView::saveVScrollBarPos() {
    //Save the scrollbar's position so we can return here after
    //a search is cleared.
    m_iSavedVScrollBarPos = verticalScrollBar()->value();
}


void WLibraryTableView::saveVScrollBarPosState() {
    //Save the vertical scrollbar position.
    int scrollbarPosition = verticalScrollBar()->value();
    m_pConfig->set(m_vScrollBarPosKey, ConfigValue(scrollbarPosition));
}

void WLibraryTableView::moveSelection(int delta) {
    QAbstractItemModel* model = this->model();
    
    while(delta != 0) {
        if(delta > 0) {
            // i is positive, so we want to move the highlight down
            int row = this->currentIndex().row();
            if (row + 1 < model->rowCount())
                this->selectRow(row + 1);

            delta--;
        } else {
            // i is negative, so we want to move the highlight up
            int row = this->currentIndex().row();
            if (row - 1 >= 0)
                this->selectRow(row - 1);

            delta++;
        }
    }
}

