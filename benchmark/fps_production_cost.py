"""Combine per-prime NTT kernel timings with each production transform histogram."""
from pathlib import Path
import csv
root=Path(__file__).resolve().parent
cost={}
for row in csv.DictReader((root/'fps_ntt_cost.csv').open()):
 cost[int(row['mod']),int(row['length']),row['direction']]=float(row['median_us'])
fields=['suite','mod','n','variant','median_us','ntt_calls','ntt_weight','ntt_estimated_us','residual_us']
with (root/'fps_production_cost.csv').open('w') as output:
 writer=csv.DictWriter(output,fieldnames=fields);writer.writeheader()
 for name in ['fps_production_ntt.csv','fps_production_crt.csv']:
  for row in csv.DictReader((root/name).open()):
   total=0.0
   for item in filter(None,row['kernels'].split(';')):
    key,counts=item.split(':');prime,length=map(int,key.split('@'));forward,inverse=map(int,counts.split('/'))
    if length==1:continue
    total+=forward*cost[prime,length,'forward']+inverse*cost[prime,length,'inverse']
   result={key:row[key] for key in fields if key in row}
   result.update(ntt_estimated_us=f'{total:.3f}',residual_us=f'{float(row["median_us"])-total:.3f}')
   writer.writerow(result)
print('Wrote fps_production_cost.csv')
