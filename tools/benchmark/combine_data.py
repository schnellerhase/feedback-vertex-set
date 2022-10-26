
import csv
import os

solvers = os.listdir('data')
solvers.sort()   

with open('data.csv', 'w+') as f:

    writer = csv.writer(f)
    writer.writerow(['track'] + [solver.replace('_', '-') for solver in solvers])
    tracks = os.listdir(f'data/naive')
    tracks.sort()
    for track in tracks:
        
        row = [track]
        for solver in solvers:    
            
            with open(f'data/{solver}/{track}') as file:
                s = file.read()
                if (s == ""):
                    continue
                if s.startswith('Command exited with non-zero status 143\n'):
                    s = s.removeprefix('Command exited with non-zero status 143\n')
                time = float(s)
                row += [time]

        writer.writerow(row)

