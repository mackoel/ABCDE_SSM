#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  permute_snp.py
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
import errno

def main(args):
	file1 = args[1]
	nperm = int(args[2])

	f = h5py.File(file1, 'r')
	gr_covar = f['gr_covar'][:]
	species = f["species"][:]
	doy = f['doy'][:]
	geo_id = f['geo_id'][:]
	gr_names = f['gr_names'][:]
	month = f['month'][:]
	emresponse = f['response_EM'][:]
	r1response = f['response_R1'][:]
	r3response = f['response_R3'][:]
	r5response = f['response_R5'][:]
	r7response = f['response_R7'][:]
	r8response = f['response_R8'][:]
	year = f['year'][:]
	f.close()

	# geno_df = pd.DataFrame(data = gr_covar, index = species, columns = gr_names)
	for gg in range(0, len(gr_names)):
		gr = gr_names[gg]
		fl = 'v4_with_snp_snp' + str(gg)
		try:
			os.makedirs(fl)
		except OSError as exc:  # Python ≥ 2.5
			if exc.errno == errno.EEXIST and os.path.isdir(fl):
				pass
	        # possibly handle other errno cases here, otherwise finally:
			else:
				raise
		gr_covar_copy = gr_covar.copy()
		for ii in range(0, nperm):
			file3 = fl + '/' + os.path.splitext(file1)[0] + '_' + str(ii) + '.h5'
			print(file3)
			idx = np.random.permutation(gr_covar_copy.shape[0])
			print(gr_covar_copy[:, gg])
			gr_covar_copy[:, gg] = gr_covar_copy[idx, gg]
			print(gr_covar_copy[:, gg])
			new_file = h5py.File(file3, 'w')
			new_file['doy'] = doy
			new_file['gr_covar'] = gr_covar_copy
			new_file['geo_id'] = geo_id
			new_file['gr_names'] = gr_names
			new_file['month'] = month
			new_file['response_EM'] = emresponse
			new_file['response_R1'] = r1response
			new_file['response_R3'] = r3response
			new_file['response_R5'] = r5response
			new_file['response_R7'] = r7response
			new_file['response_R8'] = r8response
			new_file['species'] = species
			new_file['year'] = year
			new_file.close()

	return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
