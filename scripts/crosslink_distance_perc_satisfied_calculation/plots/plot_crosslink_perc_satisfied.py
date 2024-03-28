import matplotlib.pyplot as plt
import numpy as np
import os
import sys
import matplotlib.patches as mpatches

def plot_crosslink_satisfaction(ax, file_path, label, color, name):
    percentages = []
    with open(file_path, 'r') as file:
        for line in file:
            percentage = float(line.split(' ')[1])
            percentages.append(percentage)

    # ax.hist(percentages, bins=10, color=color, label=label, alpha=0.7, histtype='step',linewidth=2, density=True)
    ax.violinplot(percentages, showmeans=False, showmedians=False)
    ax.set_title(f'{name}')
    ax.set_ylabel('Percentage',fontsize=14)
    ax.set_xlabel('Density',fontsize=14)
    ax.tick_params(axis='both', which='major', labelsize=12)

def file_parsing(name, ax):
    file1_path = os.path.join('/home/muskaan/easal/imp_output/xl_satisfaction/', name + '_perc_satisfied.txt')
    file2_path = os.path.join('/home/muskaan/easal/easal_output/xl_satisfaction/', name + '_perc_satisfied.txt')

    plot_crosslink_satisfaction(ax, file1_path, 'IMP', color='blue', name=name)
    plot_crosslink_satisfaction(ax, file2_path, 'EASAL', color='orange', name=name)

input_cases = ["1dfj_DSSO_3", "1clv_DSSO_2", "1kxp_DSSO_4", "1r0r_DSSO_3", "2ayo_DSSO_4", "2b42_DSSO_5", "2hle_DSSO_5"] #DSSO less than 5
# input_cases = ["1dfj_DSSO_9", "1clv_DSSO_6", "1kxp_DSSO_7", "1r0r_DSSO_7", "2ayo_DSSO_8", "2b42_DSSO_10", "2hle_DSSO_10"] #DSSO 6-10
# input_cases = ["1dfj_DSSO_12", "1kxp_DSSO_11", "2ayo_DSSO_13", "2hle_DSSO_14"] #DSSO more than 10
# input_cases = ["gata_gatc_DSSO_3", "gcvpa_gcvpb_DSSO_5","roca_putc_DSSO_2", "sucd_succ_DSSO_4","phes_phet_DSSO_8"] #DSSO experimental
# input_cases = ["1dfj_EDC_4", "1clv_EDC_8", "1kxp_EDC_7", "1r0r_EDC_6", "2ayo_EDC_5", "2b42_EDC_10", "2hle_EDC_9"] #EDC

fig, axs = plt.subplots(3, 3, figsize=(20, 20), gridspec_kw={'wspace': 0.5, 'hspace': 0.5})

for idx, case in enumerate(input_cases):
    row = idx // 3
    col = idx % 3
    file_parsing(case, axs[row, col])
    axs[row, col].legend(handles=[mpatches.Patch(color='blue'), mpatches.Patch(color='orange')], labels=['IMP', 'EASAL'])

plt.savefig(f'/home/muskaan/easal/plots/percentage_satisfied/{sys.argv[1]}.png')
plt.show()
