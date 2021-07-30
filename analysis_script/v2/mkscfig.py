#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  mkscfig.py
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

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def main(args):
	allfile = args[1]
	testfile = args[2]
	figfile = args[3]

	fdf = pd.read_csv(allfile, sep=',')
	y_data = fdf['event_day_R1'].to_numpy()
	y_train = fdf['dtR1'].to_numpy()
	fdf = pd.read_csv(testfile, sep=',')
	y_tdata = fdf['event_day_R1'].to_numpy()
	y_test = fdf['dtR1'].to_numpy()

	dpi = 80
	fig = plt.figure(dpi = dpi, figsize = (640 / dpi, 480 / dpi) )
	ax = plt.axes()
	ax.yaxis.grid(True, zorder = 1)
	plt.xlabel('Time to flowering')
	plt.ylabel('Prediction')

	plt. plot(y_data, y_data, label = "x=y", color = 'black')
	plt. scatter(y_data, y_train, label = "Training", color = 'grey')
	plt. scatter(y_tdata, y_test, label = "Test", color = 'green')
	plt.legend(loc='upper left')
	plt.show()

	fig.savefig(figfile)

	return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
