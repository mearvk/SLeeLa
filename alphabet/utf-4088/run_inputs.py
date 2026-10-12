#!/usr/bin/env python3
"""Execute the five curated UTF-4088 alphabet probe JSON fixtures."""
import json, math, xml.etree.ElementTree as ET
from pathlib import Path
HERE=Path(__file__).resolve().parent
AXES=["merit","time","space","importance","eccentricity","cause","distance","definability","relation"]
MASK=(1<<64)-1
def mix64(z):
 z&=MASK;z^=z>>30;z=(z*0xBF58476D1CE4E5B9)&MASK;z^=z>>27;z=(z*0x94D049BB133111EB)&MASK;z^=z>>31;return z&MASK
def q(v):
 v=max(-1e9,min(1e9,float(v)));n=math.floor(v*1_000_000+0.5) if v>=0 else math.ceil(v*1_000_000-0.5);return int(n)&MASK
def symbol_key(x,y,p,v):
 h=0x9E3779B97F4A7C15
 for val,off in [(x,0x100000001B3),(y,0x9E3779B97F4A7C15),(p,0xBF58476D1CE4E5B9),(v,0x94D049BB133111EB)]:h^=mix64((q(val)+off)&MASK)
 return mix64(h)
def validate(i,rid):
 assert i["schema"]=="utf4088-alphabet-probe-input-v1" and i["reference_set_id"]==rid
 assert isinstance(i["input_id"],int) and 1<=i["input_id"]<=5
 l=i["location"];p=i["pressure"]["iteration"];v=i["field"]["voltage_normalized"];t=i["time"]["logical_seconds"]
 assert all(isinstance(l[k],(int,float)) and 0<=l[k]<=1 for k in ("x","y"))
 assert isinstance(l["z_layer"],int) and l["z_layer"]>=0 and isinstance(p,int) and 1<=p<=50
 assert isinstance(v,(int,float)) and 0<=v<=1 and isinstance(t,(int,float)) and t>=0
 assert set(i["semantic_profile"])==set(AXES) and all(0<=v<=1 for v in i["semantic_profile"].values())
def wording(scores):
 order=sorted(AXES,key=lambda a:(-scores[a],AXES.index(a)));pair=frozenset(order[:2])
 presets={frozenset(("space","distance")):("spatial distance","distantia spatialis"),frozenset(("time","relation")):("temporal relation","relatio temporalis"),frozenset(("cause","definability")):("causal definability","definibilitas causalis"),frozenset(("merit","importance")):("merit and importance","meritum et momentum"),frozenset(("eccentricity","distance")):("eccentric distance","distantia eccentrica")}
 en,la=presets.get(pair,(order[0]+" and "+order[1],order[0]+" et "+order[1]))
 return {"top_axes":order[:2],"ranked_axes":[{"axis":a,"score":round(scores[a],6)} for a in order],"anglophone_wording":en,"latin_wording":la,"wording_status":"provisional generated wording; not a verified dictionary entry"}
def xml_write(obj,path,root_name,attrs=None):
 def add(parent,key,val):
  if isinstance(val,dict):
   el=ET.SubElement(parent,key)
   for k,v in val.items():add(el,k,v)
  elif isinstance(val,list):
   el=ET.SubElement(parent,key)
   for v in val:add(el,"item",v)
  else:
   el=ET.SubElement(parent,key)
   if val is None:el.set("nil","true")
   else:el.text=str(val).lower() if isinstance(val,bool) else str(val)
 root=ET.Element(root_name,attrs or {})
 for k,v in obj.items():add(root,k,v)
 ET.indent(root,space="  ");ET.ElementTree(root).write(path,encoding="unicode",xml_declaration=True)
def run(inp,ref,prev):
 validate(inp,ref["reference_set_id"]);loc=inp["location"];p=inp["pressure"]["iteration"];v=float(inp["field"]["voltage_normalized"]);t=float(inp["time"]["logical_seconds"])
 pt={"x":float(loc["x"]),"y":float(loc["y"]),"normalized_depth":(p-1)/49.0,"normalized_voltage":v};prof={a:float(inp["semantic_profile"][a]) for a in AXES};all_terms=[]
 pol=ref["distance_policy"]
 for term in ref["terms"]:
  a=term["anchor_4d"];d=math.sqrt(sum((pt[k]-float(a[k]))**2 for k in pt));gp=math.exp(-d/pol["geometry_bandwidth"]);sp=1-sum(abs(prof[k]-term["semantic_signature"][k]) for k in AXES)/len(AXES);cp=pol["composite_weights"]["geometry_proximity"]*gp+pol["composite_weights"]["semantic_proximity"]*sp
  all_terms.append({"term_id":term["term_id"],"concept":term["concept"],"english":term["english"],"latin":term["latin"],"latin_status":term["latin_status"],"euclidean_distance_4d":d,"geometry_proximity":gp,"semantic_proximity":sp,"composite_proximity":cp,"time_delta_seconds":abs(t-term["reference_time_seconds"])})
 geo=sorted(all_terms,key=lambda a:(a["euclidean_distance_4d"],a["term_id"]));rank=sorted(all_terms,key=lambda a:(-a["composite_proximity"],a["euclidean_distance_4d"],a["term_id"]));gap=rank[0]["composite_proximity"]-rank[1]["composite_proximity"];key=symbol_key(pt["x"],pt["y"],p,v)
 return {"result_schema":"utf4088-alphabet-probe-result-v1","input_id":inp["input_id"],"input_name":inp["input_name"],"execution":{"engine":"python-reference-probe-v1","status":"executed","reference_set_id":ref["reference_set_id"],"native_cpp_equivalence":"not_verified","meaning_validation":"not_performed"},"symbol":{"derived_symbol_key_u64":"0x%016X"%key,"is_unicode_codepoint":False,"registry_status":"provisional candidate; not published or semantically validated"},"coordinates":{"location":{"x":pt["x"],"y":pt["y"],"z_layer":loc["z_layer"]},"pressure_iteration":p,"normalized_depth":pt["normalized_depth"],"voltage_normalized":v,"space_time_vector_4d":pt,"logical_time_seconds":t,"delta_time_seconds_from_previous_input":None if prev is None else t-prev,"z_layer_and_time_are_not_dimensions_in_4d_match_distance":True},"semantic_profile":{"status":"provisional curated scores; not measured linguistic data","scores":prof,"overall_merit_score":0.5*(prof["merit"]+prof["importance"])},"wording":wording(prof),"matching":{"geometry_metric":"4D Euclidean distance on x,y,normalized_depth,normalized_voltage","geometry_proximity_kernel":"exp(-distance / 0.25)","composite_formula":"0.65 * geometry_proximity + 0.35 * semantic_proximity","nearest_geometric_term":geo[0],"nearest_composite_term":rank[0],"runner_up_composite_term":rank[1],"composite_margin":gap,"match_status":"provisional-leading-match" if gap>=0.05 else "ambiguous-provisional-match","top_5_by_composite":rank[:5],"all_terms_by_composite":rank},"interpretation":"Coordinates, weights, and reference anchors are curated demonstration inputs. Proximity is a numeric result of this declared prototype, not proof that a glyph has an established natural-language meaning."}
def md_result(r):
 c,m,w,s=r["coordinates"],r["matching"],r["wording"],r["symbol"];dt=c["delta_time_seconds_from_previous_input"];dt="—" if dt is None else f"{dt:.2f}"
 lines=[f"# UTF-4088 Probe Result {r['input_id']}","",f"**Input:** {r['input_name']}","",f"**Execution:** {r['execution']['status']} (Python reference prototype)",f"**Match status:** {m['match_status']}","","## Input and coordinate values","","| Field | Value |","|---|---:|",f"| x | {c['location']['x']:.6f} |",f"| y | {c['location']['y']:.6f} |",f"| z-layer (traversal metadata) | {c['location']['z_layer']} |",f"| pressure/depth iteration | {c['pressure_iteration']} / 50 |",f"| normalized depth | {c['normalized_depth']:.6f} |",f"| normalized voltage (synthetic) | {c['voltage_normalized']:.6f} |",f"| logical time (seconds) | {c['logical_time_seconds']:.2f} |",f"| delta from previous input (seconds) | {dt} |","| 4D feature vector | x, y, normalized depth, normalized voltage |","","## Candidate symbol and wording","","- Derived symbol key: "+s["derived_symbol_key_u64"]+" (64-bit key; not a Unicode code point)","- Anglophone wording: "+w["anglophone_wording"],"- Latin wording: "+w["latin_wording"],"- Dominant axes: "+", ".join(w["top_axes"]),f"- Prototype merit score: {r['semantic_profile']['overall_merit_score']:.3f}","","## Proximity to curated reference terms","","| Rank | Reference term | English | Latin | 4D distance | Geometry proximity | Semantic proximity | Composite proximity | Time delta (s, informational) |","|---:|---|---|---|---:|---:|---:|---:|---:|"]
 for n,a in enumerate(m["top_5_by_composite"],1):lines.append(f"| {n} | {a['concept']} ({a['term_id']}) | {a['english']} | {a['latin']} | {a['euclidean_distance_4d']:.6f} | {a['geometry_proximity']:.6f} | {a['semantic_proximity']:.6f} | {a['composite_proximity']:.6f} | {a['time_delta_seconds']:.2f} |")
 lines += ["",f"Nearest geometric anchor: {m['nearest_geometric_term']['concept']}, 4D distance {m['nearest_geometric_term']['euclidean_distance_4d']:.6f}.",f"Nearest composite anchor: {m['nearest_composite_term']['concept']}, margin over runner-up {m['composite_margin']:.6f}.","","## Interpretation and limits","",r["interpretation"],"","- Depth is an abstract iteration, not physical pressure. Normalized voltage is synthetic, not measured volts.","- Logical time and z-layer are recorded but excluded from the 4D matching metric.","- Latin/English labels are provisional. Definibilitas is a Latinized technical coinage, not claimed as classical Latin.","- The key is not a Unicode scalar or registered UTF-4088 code point.","- C++ equivalence and semantic validity have not been verified.",""]
 return "\n".join(lines)
def main():
 root=HERE;ind=root/"inputs";outd=root/"results";outd.mkdir(exist_ok=True);ref=json.loads((root/"reference-terms.json").read_text());files=sorted(ind.glob("input-*.json"),key=lambda f:int(f.stem.split("-")[1]))
 if [int(f.stem.split("-")[1]) for f in files]!=[1,2,3,4,5]:raise SystemExit("expected input-1.json through input-5.json")
 results=[];prev=None
 for f in files:
  i=json.loads(f.read_text());r=run(i,ref,prev);prev=i["time"]["logical_seconds"];results.append(r);n=r["input_id"]
  (outd/f"result-{n}.json").write_text(json.dumps(r,indent=2,ensure_ascii=False)+"\n");xml_write(r,outd/f"result-{n}.xml","utf4088AlphabetProbeResult",{"inputId":str(n),"schema":r["result_schema"]});(outd/f"result-{n}.md").write_text(md_result(r))
 rows=[]
 for r in results:
  c,m,w=r["coordinates"],r["matching"],r["wording"];rows.append({"input_id":r["input_id"],"input_name":r["input_name"],"symbol_key":r["symbol"]["derived_symbol_key_u64"],"english_wording":w["anglophone_wording"],"latin_wording":w["latin_wording"],"z_layer":c["location"]["z_layer"],"x":c["location"]["x"],"y":c["location"]["y"],"pressure_iteration":c["pressure_iteration"],"normalized_depth":c["normalized_depth"],"normalized_voltage":c["voltage_normalized"],"logical_time_seconds":c["logical_time_seconds"],"nearest_geometric_term":m["nearest_geometric_term"]["concept"],"nearest_geometric_distance_4d":m["nearest_geometric_term"]["euclidean_distance_4d"],"nearest_composite_term":m["nearest_composite_term"]["concept"],"composite_proximity":m["nearest_composite_term"]["composite_proximity"],"composite_margin":m["composite_margin"],"match_status":m["match_status"],"overall_merit_score":r["semantic_profile"]["overall_merit_score"]})
 summ={"schema":"utf4088-alphabet-probe-summary-v1","execution_status":"executed","engine":"python-reference-probe-v1","reference_set_id":ref["reference_set_id"],"input_count":5,"native_cpp_equivalence":"not_verified","meaning_validation":"not_performed","metric_policy":ref["distance_policy"],"rows":rows,"limits":["Reference-term anchors and semantic signatures are curated prototype values.","Latin and English labels are provisional wordings, not validated philological output.","Proximity scores are model outputs, not factual probabilities.","Native C++ engine equivalence is not verified."]}
 (outd/"summary.json").write_text(json.dumps(summ,indent=2,ensure_ascii=False)+"\n");xml_write(summ,outd/"summary.xml","utf4088AlphabetProbeSummary",{"schema":summ["schema"],"inputCount":"5"})
 lines=["# UTF-4088 Five-Input Probe Results","","Status: executed by Python reference prototype; native C++ equivalence is not verified and semantics have not been validated.","","## Summary table","","| # | Location (z, x, y) | Depth | Norm. voltage | Logical time (s) | English wording | Latin wording | Nearest 4D anchor | 4D distance | Nearest composite anchor | Composite score | Margin | Match status | Merit score |","|---:|---|---:|---:|---:|---|---|---|---:|---|---:|---:|---|---:|"]
 for r in rows:lines.append(f"| {r['input_id']} | ({r['z_layer']}, {r['x']:.2f}, {r['y']:.2f}) | {r['pressure_iteration']} | {r['normalized_voltage']:.2f} | {r['logical_time_seconds']:.2f} | {r['english_wording']} | {r['latin_wording']} | {r['nearest_geometric_term']} | {r['nearest_geometric_distance_4d']:.6f} | {r['nearest_composite_term']} | {r['composite_proximity']:.6f} | {r['composite_margin']:.6f} | {r['match_status']} | {r['overall_merit_score']:.3f} |")
 lines += ["","## Algorithm","","- Normalized depth: (iteration - 1) / 49.","- Four matching dimensions: x, y, normalized depth, normalized voltage. Z-layer and logical time are metadata, not matching dimensions.","- Geometry proximity: exp(-4D Euclidean distance / 0.25).","- Semantic proximity: one minus mean absolute difference across nine curated axes.","- Composite proximity: 0.65 × geometry proximity + 0.35 × semantic proximity.","- Time deltas to term anchors are informational only and do not alter rankings.","","## Semantic axes","","| Axis | Provisional interpretation |","|---|---|","| merit | merit relevance |","| time | temporal relevance |","| space | spatial relevance |","| importance | importance relevance |","| eccentricity | eccentricity/distinctiveness |","| cause | causal relevance |","| distance | spatial/conceptual separation |","| definability | how readily a concept can be specified |","| relation | relational relevance |","","Scores are curated prototype weights, not measured properties, trained probabilities, or objective judgments of value.","","## Limitations",...summ["limits"].map(x=>"- "+x),"","See result-1 through result-5 in JSON, XML, and Markdown for full ranked lists.",""]
 (outd/"summary.md").write_text("\n".join(lines))
 assert len(results)==5 and len(list(outd.glob("result-*.json")))==5 and len(list(outd.glob("result-*.xml")))==5 and len(list(outd.glob("result-*.md")))==5
 for n in range(1,6):json.loads((outd/f"result-{n}.json").read_text());ET.parse(outd/f"result-{n}.xml")
 json.loads((outd/"summary.json").read_text());ET.parse(outd/"summary.xml")
 print("PASS: 5 JSON inputs; 15 individual result files; summary JSON/XML/MD; JSON and XML parse checks passed")
if __name__=="__main__":main()
