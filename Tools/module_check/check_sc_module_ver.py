import xml.etree.ElementTree as ET
import sys
# XMLファイルのパス
xml_file_path = '.\\Projects\\aws_ryz014a_ck_rx65n\\e2studio_ccrx\\aws_ryz014a_ck_rx65n.scfg'

print(">>> Checking following project of FIT modules: " +'.\\Projects\\aws_ryz014a_ck_rx65n\\e2studio_ccrx\\aws_ryz014a_ck_rx65n.scfg')
# 期待値のリスト
from expect_module_lists import expected_values
#expected_values = [
#    {"display": "FreeRTOS_Kernel", "version": "1.0.120aa"},
#    {"display": "FreeRTOS_Object", "version": "1.0.120"},
#    {"display": "r_bsp", "version": "7.20"},
#    {"display": "r_byteq", "version": "2.00"},
#    {"display": "r_irq_rx", "version": "4.00"},
#    {"display": "r_s12ad_rx", "version": "5.00"},
#    {"display": "r_sci_rx", "version": "4.40"},
#    {"display": "r_tsip_rx", "version": "1.17.l"},
#   {"display": "r_fwup", "version": "2.00"},
#    {"display": "r_cellular", "version": "1.11"},
#    {"display": "r_flash_rx", "version": "5.00"},
    # 他の期待値を追加することができます
#]

# XMLファイルをパース
tree = ET.parse(xml_file_path)
root = tree.getroot()

# "component"要素を探し、displayとversionを取得して比較
component_elements = root.findall(".//component")
found_matches = False

# 一致しなかったdisplayのリスト
not_matched_displays = []

# チェック結果のフラグ（0のときOK,1以上NG)
test_result = 0

for component in component_elements:
    display = component.get("display")
    version = component.get("version")
    
    # 期待値のリストと比較
    match_found = False
    for expected_value in expected_values:
        if display == expected_value["display"] and version == expected_value["version"]:
            print(f"Found FIT modules: display='{display}', version='{version}'")
            match_found = True
    
    # 一致しなかった場合、displayをリストに追加
    if not match_found:
        not_matched_displays.append(display) 
        not_matched_displays.append(version)
        test_result += 1

# 一致しなかったdisplayの情報を表示
if not_matched_displays:
    print("Not Match FIT modules:")
    for display in not_matched_displays:
        print(display)
    print("Please check the module and the version are correct.")
    

# 期待値リストにあるがXML内にないItemを表示
for expected_value in expected_values:
    expected_display = expected_value["display"]
    if expected_display not in [component.get("display") for component in component_elements]:
        print(f"Not match {expected_display} in XML of SmartConfigurator.")
        test_result +=1;

if test_result == 0:
    #正常終了
    print("OK module version")
    sys.exit(0)
else:
    #異常終了
    print("NG module version")
    sys.exit(-1)
    

