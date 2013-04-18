// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DOCKWIDGET_H
# define DOCKWIDGET_H

#include <QDockWidget>

class DockWidget :
  public QDockWidget
{
  Q_OBJECT

  public:
  explicit DockWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
  explicit DockWidget(const QString& title,
		      QWidget *parent = 0, Qt::WindowFlags flags = 0);

public slots:
  void switchVisibility();
};

#endif // DOCKWIDGET_H
