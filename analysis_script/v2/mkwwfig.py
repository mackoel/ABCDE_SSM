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
import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

good_const = 12.0

def plot_cw_fig(figfile, datarrs, markers, case):
	dpi = 80
	fig = plt.figure(dpi = dpi, figsize = (640 / dpi, 480 / dpi) )
# Creating axes instance
	# ax = fig.add_axes([0, 0, 1, 1])
	ax = plt.axes()
	ax.yaxis.grid(True, zorder = 1)
# Creating plot
#	bp = ax.boxplot(datarrs)
	plt.xlabel('Temperature increment')
	plt.ylabel('Difference between prediction and data')
	xs = range(1, len(markers) + 1)
	for row in datarrs:
		if (case == -1):
			if (abs(max(row) - min(row)) > 10):
				plt. plot(xs, row, label = "line 1")
		if (case == 0):
			plt. plot(xs, row, label = "line 1")
		if (case == 1):
			if (max(abs(row)) < good_const and abs(max(row) - min(row)) > 10 and row[0] - row[len(markers) - 1] > 5):
				plt. plot(xs, row, label = "line 1")
		elif (case == 2):
			if (max(abs(row)) < good_const and abs(max(row) - min(row)) > 10 and row[len(markers) - 1] - row[0] > 5):
				plt. plot(xs, row, label = "line 1")
		elif (case == 3):
			if (max(abs(row)) < good_const and abs(max(row) - min(row)) > 10 and abs(row[len(markers) - 1] - row[0]) < 5):
				plt. plot(xs, row, label = "line 1")
		elif (case == 4):
			if (max(abs(row)) < good_const and abs(max(row) - min(row)) > 10 and row[int((len(markers) - 1)/2)] - row[0] > 1 and row[int((len(markers) - 1)/2)] - row[len(markers) - 1] > 1):
				plt. plot(xs, row, label = "line 1")
		elif (case == 5):
			if (max(abs(row)) < good_const and abs(max(row) - min(row)) > 10 and row[0] - row[int((len(markers) - 1)/2)] > 1 and row[len(markers) - 1] - row[int((len(markers) - 1)/2)] > 1):
				plt. plot(xs, row, label = "line 1")
		elif (case == 6):
			if (max(abs(row)) >= good_const):
				plt. plot(xs, row, label = "line 1")
	plt.xticks(xs, markers)
	fig.autofmt_xdate(rotation = 25)
# show plot
	plt.show()
	fig.savefig(figfile)

def get_difday_for_step(ff, flag = 1):
	fdf = pd.read_csv(ff, sep=',')
	y_data = fdf['event_day_R1'].to_numpy()
	y_model = fdf['dtR1'].to_numpy()
	rmscore = y_model
	if (flag == 1):
		rmscore = y_model - y_data
	return(rmscore)

def main(args):
	filefig = args[1]
	t_list = args[2]
	mm = int(args[3])
	case = int(args[4])
	flag = int(args[5])
	datarrs = list()
	ttk = t_list.split(',')
	for t in ttk:
		arg = float(t)
		file3 =  'weather' + str(mm) + '_' + str(t) + '_stack_output.csv'
		datarr = get_difday_for_step(file3, flag)
		datarrs.append(datarr)
	print(datarrs)
	datarrs = np.asarray(datarrs).T
	print(datarrs)
	plot_cw_fig(filefig, datarrs, ttk, case)
	return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
