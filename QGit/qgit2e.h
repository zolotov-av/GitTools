/******************************************************************************
 * This file is part of the libqgit2e library, fork of libqgit2
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
 * Copyright (C) 2013 Leonardo Giordani
 * Copyright (c) 2019 Alex V. Zolotov <shade@shamangrad.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef INCLUDE_QGIT2E_H
#define INCLUDE_QGIT2E_H

#define LIBQGIT2E_VERSION "0.26.1"
#define LIBQGIT2E_VER_MAJOR 0
#define LIBQGIT2E_VER_MINOR 26
#define LIBQGIT2E_VER_REVISION 1

#define LIBQGIT2E_SOVERSION 1

#include "src/qgitblob.h"
#include "src/qgitcheckoutoptions.h"
#include "src/qgitcherrypickoptions.h"
#include "src/qgitcommit.h"
#include "src/qgitconfig.h"
#include "src/qgitcredentials.h"
#include "src/qgitdatabase.h"
#include "src/qgitdatabasebackend.h"
#include "src/qgitdiff.h"
#include "src/qgitdiffdelta.h"
#include "src/qgitdifffile.h"
#include "src/qgitexception.h"
#include "src/qgitglobal.h"
#include "src/qgitindex.h"
#include "src/qgitindexentry.h"
#include "src/qgitindexmodel.h"
#include "src/qgitmergeoptions.h"
#include "src/qgitobject.h"
#include "src/qgitoid.h"
#include "src/qgitref.h"
#include "src/qgitremote.h"
#include "src/qgitrepository.h"
#include "src/qgitrevwalk.h"
#include "src/qgitsignature.h"
#include "src/qgitstatus.h"
#include "src/qgitstatusentry.h"
#include "src/qgitstatuslist.h"
#include "src/qgitstatusoptions.h"
#include "src/qgittag.h"
#include "src/qgittree.h"
#include "src/qgittreeentry.h"

#endif
