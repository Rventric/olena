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

#ifndef RUNNER_H
# define RUNNER_H

#include <mln/core/image/image2d.hh>
#include <mln/io/magick/load.hh>
#include <mln/value/rgb8.hh>
#include <QStringList>
#include <QMessageBox>
#include <QThread>
#include <QFile>

#include "preprocess.hh"
#include "process.hh"
#include "configs.hh"
#include "region.hh"
#include "dir.hh"

using namespace mln;

enum RunMode
{
    Demat,
    Export
};

class Runner :
        public QThread
{
        Q_OBJECT

    public:
        explicit Runner(QObject *parent = 0);
        void start_demat(const QStringList& filenames);
        void start_export(const QString& imgfile, const QString& xmlfile, const QString& outfile);

    private:
        image2d<bool> preprocess(const image2d<value::rgb8>& ima);
        void process(const image2d<value::rgb8>& original_ima, const image2d<bool>& processed_ima, int arg);
        void export_as();
        virtual void run();
        template <typename V> unsigned find_best_scale(const mln::image2d<V>& ima);

        QStringList args_;
        RunMode mode_;

    public slots:
        void stop();

    signals:
        void new_step(const QString& step_name);
        void new_progress_max_value(int i);
        void new_progress_label(const QString& msg);
        void xml_saved(const QString& filename);
        void progress();
        void finished();
};

#endif // RUNNER_H
