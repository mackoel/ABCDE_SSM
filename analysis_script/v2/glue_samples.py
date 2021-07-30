#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  glue_samples.py
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

def main(args):
	file1= args[1]
	file2= args[2]
	file3= args[3]

	f = h5py.File(file1, 'r')
	gr_covar1 = f['gr_covar'][:]
	species1 = f["species"][:]
	doy1 = f['doy'][:]
	geo_id1 = f['geo_id'][:]
	gr_names1 = f['gr_names'][:]
	month1 = f['month'][:]
	emresponse1 = f['response_EM'][:]
	r1response1 = f['response_R1'][:]
	r3response1 = f['response_R3'][:]
	r5response1 = f['response_R5'][:]
	r7response1 = f['response_R7'][:]
	r8response1 = f['response_R8'][:]
	year1 = f['year'][:]
	f.close()

	f = h5py.File(file2, 'r')
	gr_covar2 = f['gr_covar'][:]
	species2 = f["species"][:]
	doy2 = f['doy'][:]
	geo_id2 = f['geo_id'][:]
	gr_names2 = f['gr_names'][:]
	month2 = f['month'][:]
	emresponse2 = f['response_EM'][:]
	r1response2 = f['response_R1'][:]
	r3response2 = f['response_R3'][:]
	r5response2 = f['response_R5'][:]
	r7response2 = f['response_R7'][:]
	r8response2 = f['response_R8'][:]
	year2 = f['year'][:]
	f.close()

	new_file = h5py.File(file3, 'w')
	new_file['doy'] = np.concatenate((doy1, doy2), axis = 0)
	new_file['gr_covar'] = np.concatenate((gr_covar1, gr_covar2), axis = 0)
	new_file['geo_id'] = np.concatenate((geo_id1, geo_id2), axis = 0)
	new_file['gr_names'] = gr_names1
	new_file['month'] = np.concatenate((month1, month2), axis = 0)
	new_file['response_EM'] = np.concatenate((emresponse1, emresponse2), axis = 0)
	new_file['response_R1'] = np.concatenate((r1response1, r1response2), axis = 0)
	new_file['response_R3'] = np.concatenate((r3response1, r3response2), axis = 0)
	new_file['response_R5'] = np.concatenate((r5response1, r5response2), axis = 0)
	new_file['response_R7'] = np.concatenate((r7response1, r7response2), axis = 0)
	new_file['response_R8'] = np.concatenate((r8response1, r8response2), axis = 0)
	new_file['species'] = np.concatenate((species1, species2), axis = 0)
	new_file['year'] = np.concatenate((year1, year2), axis = 0)
	new_file.close()

	return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
