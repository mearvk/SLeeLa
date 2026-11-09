#!/usr/bin/env python3
"""Regression tests for visibility and constructor inheritance auditing."""
import json, subprocess, sys, tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
TOOL=ROOT/"lib/java/tools/java_api_visibility_constructor_audit.py"
def put(root,path,body):
 p=root/path; p.parent.mkdir(parents=True,exist_ok=True); p.write_text(body,encoding="utf-8")
with tempfile.TemporaryDirectory() as td:
 root=Path(td)
 put(root,"lib/java/java/lang/Base.sleela",'''package java.lang;
public class Base {
 public Base() {}
 protected Base(String value) {}
 private Base(int secret) {}
 public void open() {}
 protected void guarded() {}
 void packageOnly() {}
 private void hidden() {}
}
''')
 put(root,"lib/java/java/lang/SamePackageChild.sleela",'''package java.lang;
public class SamePackageChild extends Base {
 public void own() {}
}
''')
 put(root,"lib/java/java/util/CrossPackageChild.sleela",'''package java.util;
import java.lang.Base;
public class CrossPackageChild extends Base {
}
''')
 put(root,"lib/java/java/lang/ImplicitChild.sleela",'''package java.lang;
class ImplicitChild extends Base {
}
''')
 out=root/"visibility.json"
 p=subprocess.run([sys.executable,str(TOOL),"--root",str(root),"--json",str(out)],capture_output=True,text=True)
 if p.returncode: raise SystemExit(p.stdout+p.stderr)
 data=json.loads(out.read_text(encoding="utf-8"))
 inherited=data["inherited_members"]
 def has(t,n): return any(x["type"]==t and x["member"]==n for x in inherited)
 assert has("java.lang.SamePackageChild","open")
 assert has("java.lang.SamePackageChild","guarded")
 assert has("java.lang.SamePackageChild","packageOnly")
 assert not has("java.lang.SamePackageChild","hidden")
 assert has("java.util.CrossPackageChild","open")
 assert has("java.util.CrossPackageChild","guarded")
 assert not has("java.util.CrossPackageChild","packageOnly")
 assert not has("java.util.CrossPackageChild","hidden")
 assert not any(x["type"]=="java.lang.SamePackageChild" and x["member"]=="Base" for x in inherited)
 assert any(x["subtype"]=="java.lang.SamePackageChild" and x["inherited"] is False and x["visibility"]=="public" for x in data["super_constructor_options"])
 assert any(x["subtype"]=="java.util.CrossPackageChild" and x["inherited"] is False and x["visibility"]=="protected" for x in data["super_constructor_options"])
 implicit=data["types"]["java.lang.ImplicitChild"]["constructors"]
 assert len(implicit)==1 and implicit[0].get("implicit_default") is True
 assert implicit[0]["visibility"]=="package"
print("java-api-visibility-constructor-suite: PASS")
