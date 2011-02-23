#include "configurationdialog.h"

#include <QVBoxLayout>
#include <QStackedLayout>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QPushButton>
#include <QScrollBar>

#include <QUiLoader>
#include <QScriptEngine>
#include <QScriptContextInfo>

/**
 * Special version of a QListWidget that has the width of the first column as
 * minimum size.
 */
class CategoryListWidget : public QListWidget {
public:
	CategoryListWidget(QWidget *parent = 0) : QListWidget(parent) {
		setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
		setSelectionMode(QAbstractItemView::SingleSelection);
		setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	}

	virtual QSize sizeHint() const {
		int width = sizeHintForColumn(0) + frameWidth() * 2 + 5;
		if (verticalScrollBar()->isVisible())
			width += verticalScrollBar()->width();
		return QSize(width, 200);
	}
};

class ConfigurationDialog::PrivateData {
public:
	QScriptEngine *engine;
	QStackedLayout *stackedLayout;
	QListWidget *listWidget;
	QHash<QString, int> widgetIndex;
	QPushButton *applyButton;
};

ConfigurationDialog::ConfigurationDialog(QScriptEngine *engine, QWidget *parent) :
	QDialog(parent)
{
	d = new PrivateData;
	d->engine = engine;

	QVBoxLayout *layout = new QVBoxLayout(this);

	QHBoxLayout *mainLayout = new QHBoxLayout();

	d->listWidget = new CategoryListWidget(this);
	connect(d->listWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(currentItemChanged(QListWidgetItem*,QListWidgetItem*)));
	mainLayout->addWidget(d->listWidget);

	d->stackedLayout = new QStackedLayout(this);
	mainLayout->addLayout(d->stackedLayout);

	layout->addLayout(mainLayout);

	QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply, Qt::Horizontal, this);
	d->applyButton = buttons->button(QDialogButtonBox::Apply);
	d->applyButton->setEnabled(false);
	connect(d->applyButton, SIGNAL(clicked()), this, SLOT(doSave()));
	connect(buttons, SIGNAL(accepted()), this, SLOT(okClicked()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

	layout->addWidget(buttons);

	this->setLayout(layout);
}

ConfigurationDialog::~ConfigurationDialog() {
	delete d;
}

QScriptValue ConfigurationDialog::addPage( const QString &name, const QString &filename, const QString &icon ) {
	QDir dir = this->baseDir();

	QUiLoader loader;
	QFile file(dir.filePath(filename));
	file.open(QFile::ReadOnly);
	QWidget *widget = loader.load(&file, this);
	file.close();

	int index = d->stackedLayout->addWidget(widget);

	QListWidgetItem *item = new QListWidgetItem(QIcon(dir.filePath(icon)), name, d->listWidget);
	QString plugin = this->currentPlugin();
	item->setData(Qt::UserRole, plugin);
	d->listWidget->addItem(item);
	d->widgetIndex[plugin] = index;

	return d->engine->newQObject(widget, QScriptEngine::QtOwnership);
}

QDir ConfigurationDialog::baseDir() const {
	QScriptContextInfo info(d->engine->currentContext()->parentContext());
	QFileInfo fileinfo(info.fileName());
	return fileinfo.dir();
}

void ConfigurationDialog::currentItemChanged( QListWidgetItem * current, QListWidgetItem * ) {
	QString plugin = current->data(Qt::UserRole).toString();
	if (d->widgetIndex.contains(plugin)) {
		d->stackedLayout->setCurrentIndex(d->widgetIndex[plugin]);
	}
}

QString ConfigurationDialog::currentPlugin() const {
	return d->engine->evaluate("__extension__").toString();
}

void ConfigurationDialog::doSave() {
	emit save();
	d->applyButton->setEnabled(false);
}

void ConfigurationDialog::okClicked() {
	this->doSave();
	this->accept();
}

void ConfigurationDialog::open() {
	this->exec();
}

void ConfigurationDialog::valueChanged() {
	d->applyButton->setEnabled(true);
}

