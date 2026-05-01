# TKJT GUID Coverage vs JT v10 Spec

Reference: *JT File Format Reference Rev C, 2019-06-06* (v10)

All standard GUIDs share the suffix `2ac8-11d1-9b-6b-00-80-c7-bb-59-97` unless noted.

## 10dd Series (v10 standard elements)

| ID       | Spec Name                           | TKJT Class                     | Status |
|----------|-------------------------------------|--------------------------------|--------|
| 10dd1001 | Base Attribute Data                 | *(data structure, no class)*   | N/A    |
| 10dd1004 | JT Object Reference Property Atom  | `JtProperty_ObjectReference`   | ✅ |
| 10dd1014 | Draw Style Attribute Element        | `JtAttribute_DrawStyle`        | ✅ |
| 10dd1019 | Floating Point Property Atom        | `JtProperty_Float`             | ✅ |
| 10dd101b | Group Node Element                  | `JtNode_Group`                 | ✅ |
| 10dd1028 | Infinite Light Attribute Element    | `JtAttribute_InfiniteLight`    | ✅ |
| 10dd102a | Instance Node Element               | `JtNode_Instance`              | ✅ |
| 10dd102b | Integer Property Atom Element       | `JtProperty_Integer`           | ✅ |
| 10dd102c | LOD Node Element                    | `JtNode_LOD`                   | ✅ |
| 10dd1030 | Material Attribute Element          | `JtAttribute_Material`         | ✅ |
| 10dd1035 | Base Node Element                   | `JtNode_Base`                  | ✅ |
| 10dd103e | Partition Node Element              | `JtNode_Partition`             | ✅ |
| 10dd1045 | Point Light Attribute Element       | `JtAttribute_PointLight`       | ✅ |
| 10dd1046 | Polyline Set Shape Node Element     | —                              | ❌ Missing |
| 10dd1048 | Polygon Set Shape Node Element      | —                              | ❌ Missing |
| 10dd104b | Base Property Atom Element          | `JtProperty_Base`              | ✅ |
| 10dd104c | Range LOD Node Element              | `JtNode_RangeLOD`              | ✅ |
| 10dd1059 | Base Shape Node Element             | `JtNode_Shape_Base`            | ✅ |
| 10dd106e | String Property Atom Element        | `JtProperty_String`            | ✅ |
| 10dd1073 | Texture Image Attribute Element     | —                              | ❌ Missing (complex: mipmaps) |
| 10dd1077 | Tri-Strip Set Shape Node Element    | `JtNode_Shape_TriStripSet`     | ✅ |
| 10dd107f | Vertex Shape Node Element           | `JtNode_Shape_Vertex`          | ⚠️ Registered with `6b-9b` (swapped vs spec `9b-6b`) |
| 10dd1083 | Geometric Transform Attribute       | `JtAttribute_GeometricTransform` | ✅ |
| 10dd1096 | Light Set Attribute Element         | `JtAttribute_LightSet`         | ✅ |
| 10dd109f | Polygon Set LOD Element             | —                              | ❌ Missing (quantized mesh) |
| 10dd10a1 | Polyline Set Shape LOD Element      | —                              | ❌ Missing (quantized mesh) |
| 10dd10ab | Tri-Strip Set Shape LOD Element     | `JtElement_ShapeLOD_TriStripSet` | ✅ |
| 10dd10b0 | Vertex Shape LOD Element            | `JtElement_ShapeLOD_Vertex`    | ⚠️ Registered with `6b-9b` (swapped vs spec `9b-6b`) |
| 10dd10c4 | Linestyle Attribute Element         | `JtAttribute_LineStyle`        | ✅ |
| 10dd10f3 | Switch Node Element                 | `JtNode_Switch`                | ✅ |

## Non-10dd GUIDs

| GUID prefix | Full GUID (approx)                                  | Spec Name                       | TKJT Class                      | Status |
|-------------|-----------------------------------------------------|---------------------------------|---------------------------------|--------|
| 873a70e0    | `873a70e0-2ac9-11d1-9b-6b-00-80-c7-bb-59-97`        | XT B-Rep Element                | `JtElement_XTBRep`              | ✅ |
| ce357244    | `ce357244-38fb-11d1-a5-06-00-60-97-bd-c6-e1`        | Part Node Element               | `JtNode_Part`                   | ✅ |
| ce357245    | `ce357245-38fb-11d1-a5-06-00-60-97-bd-c6-e1`        | Meta Data Node Element          | `JtNode_MetaData`               | ✅ |
| ce357246    | `ce357246-38fb-11d1-a5-06-00-60-97-bd-c6-e1`        | Date Property Atom Element      | `JtProperty_Date`               | ✅ |
| ce357247    | `ce357247-38fb-11d1-a5-06-00-60-97-bd-c6-e1`        | Property Proxy Meta Data Element| —                               | ❌ Missing |
| ce357249    | `ce357249-38fb-11d1-a5-06-00-60-97-bd-c6-e1`        | PMI Manager Meta Data Element   | `JtElement_MetaData_PMIManager` | ✅ |
| e0b05be5    | `e0b05be5-fbbd-11d1-a3-a7-00-aa-00-d1-09-54`        | Late Loaded Property Atom       | `JtProperty_LateLoaded`         | ✅ |

## TKJT Extra (not in spec Object Type ID table)

| GUID                                          | TKJT Class               | Note |
|-----------------------------------------------|--------------------------|------|
| `10dd10a4-2ac8-11d1-9b-6b-00-80-c7-bb-59-97` | `JtElement_ShapeLOD_Base`| GUID absent from spec Object Type ID table. Spec §7.1.4.1.1 defines Base Shape LOD Data as a common embedded data structure, not a standalone element. Possibly a TKJT-internal registration for the shared LOD base. |

## Summary

- **Spec object types:** 36 (29 in 10dd + 7 non-10dd), excluding `10dd1001` (data structure)
- **TKJT implements:** 28 of 36 spec types ✅
- **Missing from TKJT:** 6 spec types ❌
  - `10dd1046` Polyline Set Shape Node
  - `10dd1048` Polygon Set Shape Node
  - `10dd1073` Texture Image Attribute (complex: mipmap chain)
  - `10dd109f` Polygon Set LOD (quantized mesh)
  - `10dd10a1` Polyline Set Shape LOD (quantized mesh)
  - `ce357247` Property Proxy Meta Data
- **Byte-order discrepancy (⚠️):** `10dd107f` and `10dd10b0` are registered in TKJT with `6b-9b` in bytes 8–9 where the spec shows `9b-6b`. These resolve correctly in practice (real JT files match the TKJT registration), suggesting the spec or earlier OCCT implementations had a byte-swap for these two GUIDs.
- **TKJT-only registration:** `10dd10a4` (`JtElement_ShapeLOD_Base`) — no Object Type ID in spec; used as a base class dispatch point.
