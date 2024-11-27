cd /sys/devices/system/cpu/cpu0/cpufreq

echo "ondemand" > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor

cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor

# cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_governors 
conservative userspace powersave ondemand performance

/!\ 5 gouverneur et pas 6 car linux trop vieux /!\

export PATH=/home/fredlinux/Documents/OS-embarque/TP6/buildroot-2023.05/output/host/bin:$PATH

/home/fredlinux/Documents/OS-embarque/TP6/buildroot-2023.05/output/host/bin/arm-linux-gcc

arm-linux-gcc -o primeNumbers primeNumbers.c 

sudo cp primeNumbers /srv/tftp/

#tftp -g -r primeNumbers 192.168.1.1
#chmod +x primeNumbers 
#./primeNumbers 1000

fredlinux@fredlinux:~/Documents/OS-embarque/TP6/codes$ arm-linux-gcc -o prodScal prodScal.c 
fredlinux@fredlinux:~/Documents/OS-embarque/TP6/codes$ sudo cp prodScal /srv/tftp/

# tftp -g -r prodScal 192.168.1.1
# chmod +x prodScal
# ./prodScal 100

fredlinux@fredlinux:~/Documents/OS-embarque/TP6/codes$ arm-linux-gcc -o writeFile writeFile.c 
fredlinux@fredlinux:~/Documents/OS-embarque/TP6/codes$ sudo cp writeFile /srv/tftp/

# tftp -g -r writeFile 192.168.1.1
# chmod +x writeFile 
# touch testWrite.txt
# ./writeFile testWrite.txt 100
100+0 records in
100+0 records out
# vi testWrite.txt 

fredlinux@fredlinux:~/Documents/OS-embarque/TP6/codes$ arm-linux-gcc -o measureWorkloadGovernor measureWorkloadGovernor.c 
fredlinux@fredlinux:~/Documents/OS-embarque/TP6/codes$ sudo cp measureWorkloadGovernor /srv/tftp/

# tftp -g -r measureWorkloadGovernor 192.168.1.1
# chmod +x measureWorkloadGovernor
# ls
,                        measureWorkloadGovernor  testWrite.txt
blink                    mod16                    trigger
blink4                   primeNumbers             writeFile
knight_rider             prodScal
# ./measureWorkloadGovernor conservative pr
primeNumbers  prodScal
# ./measureWorkloadGovernor conservative primeNumbers 1000000
Erreur lors de l'exécution du workload: No such file or directory
Workload terminé avec le code : 1
Durée d'exécution : 7278 microsecondes
# ./measureWorkloadGovernor powersave primeNumbers 1000000
Erreur lors de l'exécution du workload: No such file or directory
Workload terminé avec le code : 1
Durée d'exécution : 6890 microsecondes
# 

fredlinux@fredlinux:~/Documents/OS-embarque/TP6/codes$ arm-linux-gcc -o run_workload run_workload.c -lm
fredlinux@fredlinux:~/Documents/OS-embarque/TP6/codes$ sudo cp run_workload /srv/tftp/

# ./run_workload ./primeNumbers 10000
Starting workload execution with the following parameters:
Workload path: ./primeNumbers
Workload parameters: 10000


[DEBUG] Command to execute: ./measureWorkloadGovernor conservative ./primeNumbers 10000
Run 7: 11515 microseconds
[DEBUG] Command to execute: ./measureWorkloadGovernor conservative ./primeNumbers 10000
Run 8: 21501 microseconds
[DEBUG] Command to execute: ./measureWorkloadGovernor conservative ./primeNumbers 10000
Run 9: 21294 microseconds
[DEBUG] Command to execute: ./measureWorkloadGovernor conservative ./primeNumbers 10000
Run 10: 11547 microseconds
Governor: conservative
Mean execution time: 19052.40 microseconds
Standard deviation: 3923.51 microseconds
-------------------------------
Workload testing completed successfully.

#fdisk -l

#mkdir -p /mnt/usb

#cp results_*.txt /mnt/usb/

#ls /mnt/usb




