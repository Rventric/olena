#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("GScribo"));
    showMaximized();

    initGraphicsRegion();
    initDialogsWidget();
    initXmlWidget();
    initRegionWidget();
    initPageWidget();
    initMenuBar();

    // Told the left dock widget (here the pages widget and the region widget) to fill the bottom left corner.
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    // Merge the region and pages widget in one tab (pages widget over the region widget).
    tabifyDockWidget(&dockRegion_, &dockPages_);

    connectWidgets();
}

MainWindow::~MainWindow()
{
    delete preferencesDialog_;
    delete aboutDialog_;
    delete progressDialog_;
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void MainWindow::initGraphicsRegion()
{
    graphicsView_.setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView_.setScene(&scene_);

    setCentralWidget(&graphicsView_);
}

void MainWindow::initDialogsWidget()
{
    preferencesDialog_ = new PreferencesDialog(this);
    aboutDialog_ = new AboutDialog(this);
    progressDialog_ = new ProgressDialog(this);
}

void MainWindow::initPageWidget()
{
    dockPages_.setWindowTitle("Pages");
    dockPages_.setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    dockPages_.setWidget(&pagesWidget_);
    dockPages_.setMinimumWidth(190);

    addDockWidget(Qt::LeftDockWidgetArea, &dockPages_);
}

void MainWindow::initRegionWidget()
{
    dockRegion_.setWindowTitle("Regions");
    dockRegion_.setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    dockRegion_.setWidget(&regionWidget_);

    addDockWidget(Qt::LeftDockWidgetArea, &dockRegion_);
}

void MainWindow::initXmlWidget()
{
    dockXml_.setWindowTitle("Xml");
    dockXml_.setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    dockXml_.setWidget(&xmlWidget_);

    addDockWidget(Qt::BottomDockWidgetArea, &dockXml_);
    dockXml_.setVisible(false);
}

void MainWindow::initMenuBar()
{
    QMenu *menuFile = ui->menuBar->addMenu(tr("File"));

    QAction *open = menuFile->addAction(tr("Open"));
    connect(open, SIGNAL(triggered()), this, SLOT(onOpen()));

    segment_ = menuFile->addAction(tr("Segment"));
    segment_->setEnabled(false);
    connect(segment_, SIGNAL(triggered()), this, SLOT(onSegment()));

    menuFile->addSeparator();

    previewPrinting_ = menuFile->addAction(tr("Preview Printing"));
    previewPrinting_->setEnabled(false);
    connect(previewPrinting_, SIGNAL(triggered()), this, SLOT(onPreviewPrint()));

    print_ = menuFile->addAction(tr("Print"));
    print_->setEnabled(false);
    connect(print_, SIGNAL(triggered()), this, SLOT(onPrint()));

    menuFile->addSeparator();

    export_ = menuFile->addAction(tr("Exportation"));
    export_->setEnabled(false);
    connect(export_, SIGNAL(triggered()), this, SLOT(onExportation()));

    menuFile->addSeparator();

    QAction *quit = menuFile->addAction(tr("Quit"));
    connect(quit, SIGNAL(triggered()), this, SLOT(close()));

    QMenu *menuAreas = ui->menuBar->addMenu(tr("Areas"));

    QAction *page = menuAreas->addAction(tr("Page"));
    page->setCheckable(true);
    page->setChecked(true);
    connect(page, SIGNAL(triggered()), &dockPages_, SLOT(switchVisibility()));

    QAction *region = menuAreas->addAction(tr("Region"));
    region->setCheckable(true);
    region->setChecked(true);
    connect(region, SIGNAL(triggered()), &dockRegion_, SLOT(switchVisibility()));

    QAction *xml = menuAreas->addAction(tr("Xml"));
    xml->setCheckable(true);
    xml->setChecked(false);
    connect(xml, SIGNAL(triggered()), &dockXml_, SLOT(switchVisibility()));

    QAction *preferences = ui->menuBar->addAction(tr("Preferences"));
    connect(preferences, SIGNAL(triggered()), this, SLOT(onPreferences()));

    QAction *about = ui->menuBar->addAction(tr("About"));
    connect(about, SIGNAL(triggered()), this, SLOT(onAbout()));
}

void MainWindow::setActionsEnabled(bool isSegmented)
{
    segment_->setEnabled(!isSegmented);
    print_->setEnabled(isSegmented);
    previewPrinting_->setEnabled(isSegmented);
}

void MainWindow::disableActions()
{
    segment_->setEnabled(false);
    print_->setEnabled(false);
    previewPrinting_->setEnabled(false);
    export_->setEnabled(false);
}

void MainWindow::connectWidgets()
{
    // Each time the scene rect change (when a new picture is loaded), we fit the scene background rectangle in the view.
    connect(&scene_, SIGNAL(sceneRectChanged(QRectF)), &graphicsView_, SLOT(fitInView(QRectF)));
    // When dragging with right click, disable selection.
    connect(&graphicsView_, SIGNAL(beginDrag()), &scene_, SLOT(disableSelection()));
    connect(&graphicsView_, SIGNAL(endDrag()), &scene_, SLOT(enableSelection()));

    // If double click on a picture of the page widget -> draw it on background scene.
    connect(&pagesWidget_, SIGNAL(imageSelectionned(QString)), this, SLOT(onFileChanged(QString)));

    // Connect the scene to the xml widget and vice versa.
    connect(&scene_, SIGNAL(newSelection(QList<RegionItem*>)), this, SLOT(onRegionSelection(QList<RegionItem*>)));
    connect(xmlWidget_.view(), SIGNAL(select(QList<XmlItem*>)), this, SLOT(onXmlSelect(QList<XmlItem*>)));
    connect(xmlWidget_.view(), SIGNAL(unselect(QList<XmlItem*>)), this, SLOT(onXmlUnselect(QList<XmlItem*>)));
    connect(xmlWidget_.view(), SIGNAL(emptySelection()), &scene_, SLOT(selectBase()));
    connect(xmlWidget_.view(), SIGNAL(resetSelection()), &scene_, SLOT(clearSelection()));

    // Connect the scene with the region widget.
    connect(&regionWidget_, SIGNAL(checkStateChanged(GraphicsRegion::Id,bool)), &scene_, SLOT(setVisible(GraphicsRegion::Id,bool)));

    // Connect the xml widget with the region widget.
    connect(&regionWidget_, SIGNAL(checkStateChanged(QString)), xmlWidget_.view(), SLOT(setFilterString(QString)));

    connect(&runner_, SIGNAL(progress()), progressDialog_, SLOT(run()));
    connect(&runner_, SIGNAL(new_progress_max_value(int)), progressDialog_, SLOT(setMaximum(int)));
    connect(&runner_, SIGNAL(new_progress_label(QString)), progressDialog_, SLOT(setLabelText(QString)));
    connect(&runner_, SIGNAL(finished()), progressDialog_, SLOT(close()));
    connect(&runner_, SIGNAL(xml_saved(QString)), this, SLOT(onXmlSaved(QString)));
}

void MainWindow::onOpen()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, "Open Image(s)", QDir::homePath(),
                                                      "Images (*.png *.jpg *.ppm *.bmp)");
    setWindowTitle(tr("GScribo"));

    if(paths.count() > 0)
    {
        QStringList filenames = pagesWidget_.filenames();
        QString path;

        int counter = 0;
        bool isContained;
        // Check for an image not already added to the page widget.
        do
        {
            path = paths[counter];
            counter++;
            isContained = filenames.contains(path);
        } while(isContained && counter < paths.count());

        if(!isContained)
        {
            pagesWidget_.addPicture(path, QPixmap(path));

            // If more than one file, we store it in the page widget.
            for(int i = counter; i < paths.count(); i++)
            {
                // Check if the page widget contains the image.
                if(!filenames.contains(paths[i]))
                {
                    path = paths[i];
                    pagesWidget_.addPicture(path, QPixmap(path));
                }
            }

            pagesWidget_.setCurrentRow(0);
            pagesWidget_.scrollToTop();
        }

        // Change current scene.
        onFileChanged(path);
    }
}

void MainWindow::onSegment()
{
    QStringList filenames;

    filenames << scene_.backgroundPath();

    // Run segmentation of page(s).
    progressDialog_->reset();
    runner_.start_demat(filenames);
}

void MainWindow::onPreviewPrint()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);
    printer.setResolution(300);

    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printScene(QPrinter*)));

    preview.exec();
}

void MainWindow::onPrint()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);
    printer.setResolution(300);

    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle("Print Document");

    if(dialog.exec() != QDialog::Accepted)
        return;

    printScene(&printer);
}

void MainWindow::printScene(QPrinter *printer)
{
    QPainter painter(printer);
    QStyleOptionGraphicsItem options;

    scene_.backgroundPixmap()->paint(&painter, &options, 0);

    // Paint backwards items first.
    printItems(&painter, scene_.root()->childsFrom(GraphicsRegion::TextRegion), &options);
    printItems(&painter, scene_.root()->childsFrom(GraphicsRegion::Image), &options);

    for(int i = GraphicsRegion::Line; i < GraphicsRegion::Image; i++)
        printItems(&painter, scene_.root()->childsFrom(static_cast<GraphicsRegion::Id>(i)), &options);

    for(int i = GraphicsRegion::Noise; i <= GraphicsRegion::Meanline; i++)
        printItems(&painter, scene_.root()->childsFrom(static_cast<GraphicsRegion::Id>(i)), &options);
}

void MainWindow::printItems(QPainter *painter, const QList<QGraphicsItem *>& items, QStyleOptionGraphicsItem *options)
{
    foreach(QGraphicsItem *child, items)
    {
        if(child->isVisible())
        {
            QRect viewport = scene_.backgroundPixmap()->mapRectFromItem(child, child->boundingRect()).toRect();
            painter->translate(abs(child->boundingRect().x() - viewport.x()),
                               abs(child->boundingRect().y() - viewport.y()));
            child->paint(painter, options);
            painter->resetTransform();
        }
    }
}

void MainWindow::onExportation()
{
    QMessageBox messageBox(this);
    messageBox.setWindowTitle("Information");
    messageBox.setText("This feature is not available yet");
    messageBox.exec();
    return;

    QFileInfo fileInfo(scene_.backgroundPath());
    QString outputSuggestion = fileInfo.baseName() + ".pdf";
    QString output = QFileDialog::getSaveFileName(0, tr("Export Document As ..."), outputSuggestion,
                                                  tr("PDF (*.pdf);; HTML (*.html *.htm"));

    if(!output.isEmpty())
    {
        progressDialog_->reset();
        runner_.start_export(scene_.backgroundPath(), xml_.filename(), output);
    }
}

void MainWindow::onXmlSaved(const QString& filename)
{
    xml_.load(filename);
    xmlWidget_.changeView(xml_.xmlItem());
    scene_.setRoot(xml_.graphicsItem());

    setActionsEnabled(true);
    export_->setEnabled(xml_.recognized());
}

void MainWindow::onFileChanged(const QString& filename)
{
    // If it's not the current scene.
    if(scene_.backgroundPath() != filename)
    {
        QString xmlPath = Xml::getPath(filename);

        if(filename.isEmpty())
        {
            xmlPath = QString();
            disableActions();
        }
        // Check if the xml file already exists.
        else if(!QFile(xmlPath).exists())
        {
            xmlPath = QString();
            setActionsEnabled(false);
        }
        else
            setActionsEnabled(true);

        xml_.load(xmlPath);
        scene_.changeScene(filename, xml_.graphicsItem());
        xmlWidget_.changeView(xml_.xmlItem());
        export_->setEnabled(xml_.recognized());
    }
}

void MainWindow::onRegionSelection(QList<RegionItem *> regionItems)
{
    if(!regionItems.isEmpty())
        xmlWidget_.view()->displayOnly(toXmlItems(regionItems));
    else
    {
        xmlWidget_.view()->reset();
        xmlWidget_.view()->displayAll();
        xmlWidget_.view()->expandAll();
    }
}

void MainWindow::onXmlChangeSelection(QList<XmlItem *> xmlItems, bool select)
{
    QList<RegionItem *> regionItems = toRegionItems(xmlItems);

    if(!regionItems.isEmpty())
    {
        if(select)
            scene_.select(regionItems);
        else
            scene_.unselect(regionItems);
    }
}

QList<RegionItem *> MainWindow::toRegionItems(QList<XmlItem *> xmlItems) const
{
    QList<RegionItem *> regionItems;
    XmlItem *child;
    foreach(child, xmlItems)
    {
        if(child->regionItem())
            regionItems << child->regionItem();
    }

    return regionItems;
}

QList<XmlItem *> MainWindow::toXmlItems(QList<RegionItem *> regionItems) const
{
    QList<XmlItem *> xmlItems;
    RegionItem *child;
    foreach(child, regionItems)
        xmlItems << child->xmlItem();

    return xmlItems;
}