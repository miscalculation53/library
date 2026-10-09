"""Generate instrumented production copies in /tmp."""
from pathlib import Path
import re
root=Path(__file__).resolve().parents[1]
out=Path('/tmp/production-profile');out.mkdir(exist_ok=True)
files=['math/fps/internal_ntt.hpp','math/fps/fps.hpp','math/convolution/middle_product.hpp','math/fps/multipoint_evaluation.hpp','math/fps/interpolation.hpp','math/fps/composition.hpp']
names={p:out/Path(p).name for p in files}
for p in files:
    source=(root/p).read_text()
    def include(m):
        path=(root/p).parent/m[1]
        path=path.resolve()
        rel=path.relative_to(root).as_posix()
        return '#include "'+str(names.get(rel,path))+'"'
    includes=[]
    def hide(m):
        includes.append(include(m));return '@INCLUDE'+str(len(includes)-1)+'@'
    source=re.sub(r'#include "([^"]+)"',hide,source)
    source=source.replace('FormalPowerSeries','ProfileFPS').replace('FPSMultipointTree','ProfileMultipointTree').replace('FPSSparseOperation','ProfileSparseOperation')
    source=re.sub(r'\bfps_([a-zA-Z_]+)',r'profile_fps_\1',source)
    for name in ['middle_product_prime','middle_product','multipoint_evaluation','interpolation','composition']:
        source=re.sub(r'\b'+name+r'\b','profile_'+name,source)
    if p.endswith('/fps.hpp'):
        source=source.replace('using F = ProfileFPS;', 'using F = ProfileFPS;\n  inline static bool force_dense = false;')
        source=re.sub(r'if \(((?:g\.)?cnt_nz\(\) <= [0-9]+)\)',r'if (!force_dense && \1)',source)
        source=source.replace('if (internal::profile_fps_use_sparse(', 'if (!force_dense && internal::profile_fps_use_sparse(')
    for i,line in enumerate(includes):source=source.replace('@INCLUDE'+str(i)+'@',line)
    (names[p]).write_text(source)
