#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  mkclimwarming.py
#
#  Copyright 2021 kkozlov <kkozlov@wave.bio.spbcas.ru>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.

import h5py
import numpy as np
import sys
import os

def main(args):
	file1 = args[1]
	t_list = args[2]
	mm = int(args[3])

	f = h5py.File(file1, 'r')
	doy1 = f['doy'][:]
	geo_id1 = f['geo_id'][:]
	month1 = f['month'][:]
	year1 = f['year'][:]
	tmax1 = f['tmax'][:]
	tmin1 = f['tmin'][:]
	rain1 = f['rain'][:]
	dl1 = f['dl'][:]
	srad1 = f['srad'][:]
	f.close()

	ttk = t_list.split(',')
	for t in ttk:
		arg = float(t)
		# print(tmin1)
		tmin = tmin1.copy()
		tmax = tmax1.copy()
		if (mm == 1):
			tmin = tmin + arg
		else:
			tmax = tmax + arg
		# print(tmin)
		file3 =  os.path.splitext(file1)[0] + str(mm) + '_' + str(t) + '.h5'
		new_file = h5py.File(file3, 'w')
		new_file['geo_id'] = geo_id1
		new_file['doy'] = doy1
		new_file['month'] = month1
		new_file['year'] = year1
		new_file['tmin'] = tmin
		new_file['tmax'] = tmax
		new_file['srad'] = srad1
		new_file['dl'] = dl1
		new_file['rain'] = rain1
		new_file.close()

	return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
