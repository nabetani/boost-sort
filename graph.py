import math
import numpy as np
from matplotlib import pyplot
from matplotlib.backends.backend_pdf import PdfPages
import csv


A4_PORTRAIT = (8.27, 11.69)
A4_LANDSCAPE = (11.69, 8.27)


def read_csv(fn):
    v = []
    with open(fn) as f:
        reader = csv.reader(f, delimiter=",", quotechar='"')
        for row in reader:
            v.append(list(row))
    return v


def write_pdf(fn):
    with PdfPages(fn) as pdf:
        fignums = pyplot.get_fignums()
        for fignum in fignums:
            pyplot.figure(fignum)
            pdf.savefig()


def uniq_items(a, ix):
    return list(set(x[ix] for x in a))


def get_ticks(data, compiler, src, sorter, sizes):
    r = []
    for size in sizes:
        for line in data:
            if line[0] == compiler and line[1] == sorter and line[2] == src and line[3] == size:
                r.append(float(line[5]))
    return r


def add_page(fig, compiler, data):
    sorters = uniq_items(data, 1)
    srces = uniq_items(data, 2)
    nsizes = sorted([float(s) for s in uniq_items(data, 3)])
    print(nsizes)
    sizes = [str(int(s)) for s in nsizes]
    fig.suptitle(compiler)
    for ix in range(len(srces)):
        src = srces[ix]
        graph = fig.add_subplot(len(srces), 1, ix+1)
        graph.set_title(src)
        graph.set_yscale('log')
        graph.set_xscale('log')

        for sorter in sorters:
            ticks = get_ticks(data, compiler, src, sorter, sizes)
            graph.plot(nsizes, ticks, label=sorter)
        graph.legend(bbox_to_anchor=(1.02, 1),
                     loc='upper left',
                     borderaxespad=0)
    pyplot.tight_layout()  # グラフが別のグラフと重なるのを抑制する
    fig.subplots_adjust(top=0.9)
    fig.align_labels()  # 同一ページ内の y-label の位置などを揃える


def main():
    all_data = read_csv("list.csv")
    compilers = uniq_items(all_data, 0)
    sorters = uniq_items(all_data, 1)
    srces = uniq_items(all_data, 2)
    sizes = uniq_items(all_data, 3)
    print("compilers", " ".join(compilers))
    print("sorters", " ".join(sorters))
    print("srces", " ".join(srces))
    print("sizes", " ".join(sizes))
    for compiler in compilers:
        fig = pyplot.figure(figsize=A4_PORTRAIT)
        add_page(fig, compiler, all_data)
    write_pdf("graph.pdf")


main()
