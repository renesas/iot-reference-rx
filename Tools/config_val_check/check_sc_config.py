import xml.etree.ElementTree as ET
import os
import re
import string
import sys

# ファイルからXMLデータを読み込む

xml_file_path = '.\\Projects\\aws_ryz014a_ck_rx65n\\e2studio_ccrx\\aws_ryz014a_ck_rx65n.scfg'

tree = ET.parse(xml_file_path)
root = tree.getroot()

# "component"要素をすべて取得
components = root.findall(".//configuration")

# "gridItem"要素を取得し、IDとSelectedIndexを1つの辞書に格納
grid_item_dict = {}
for component in components:
    description = component.get("component")
    grid_items = component.findall(".//gridItem")
    for grid_item in grid_items:
        grid_item_id = grid_item.get("id")
        grid_item_selected_index = grid_item.get("selectedIndex")
        grid_item_dict[(description, grid_item_id)] = grid_item_selected_index

# 別ファイルのマクロ定義と値
# 期待値のリスト
#from conv_macro_values import macro_values

from macro_values import conv_macro_values
#conv_macro_values = [
#    {"IRQ_CFG_PCLK_DIV64":3},
#    {"IRQ_CFG_PCLK_DIV1": 0},
#    {"BSP_CFG_PARAM_CHECKING_ENABLE": 1},
#    {"_CMT0_CMI0": 0},
#]

# マクロ定義と値を辞書に格納
#macro_dict = {item["macro"]: item["value"] for item in macro_values}

# r_configフォルダ内の.hファイルを検索
r_config_path = ".//Projects//aws_ryz014a_ck_rx65n//e2studio_ccrx//src//smc_gen//r_config"
error_count = 0
warning_count = 0

def clean_value(text):
    # 前後の空白を除去
    text = text.strip()
    # Remove ()
    text = text.replace('(', '')
    text = text.replace(')', '')
    return text

# 16進数のときtrue、文字列 or 10進数の時Falseを返す
def is_hexadecimal_or_string(input_str):
    input_str = str(input_str)
    try:
        int_value = int(input_str, 16)
        if input_str.lower().startswith('0x') and str(int_value) == input_str[2:]:
            #Hex
            return True
        else:
            #decimal
            return True
    except ValueError:
        #string
        return False

def convert_to_decimal(text):
    if type(text) == str:
        # 16進数や8進数の値を10進数に変換
        hex_match = re.match(r'0[xX][0-9a-fA-F]+', text)
        octal_match = re.match(r'0[0-7]+', text)
            
        if hex_match:
            return str(int(hex_match.group(), 16))
        elif octal_match:
            return str(int(octal_match.group(), 8))
        else:
            return text
    return text

#r_configフォルダ直下の.hを検索
for root_path, _, files in os.walk(r_config_path):
    # .hフォルダを探す
    for file_name in files:
        if file_name.endswith(".h"):
            file_path = os.path.join(root_path, file_name)
            with open(file_path, "r") as file:
                # ファイルの中身を参照
                file_contents = file.read()
                lines = file_contents.split("\n")
                # 1行ずつ読み込む
                for line in lines:
                    # /* */ のコメントを削除
                    line = line.split("/*")[0]
                    line = line.split("//")[0]
                    # #defineの行を検索
                    if line.startswith("#define"):
                        # 区切り文字SpaceはNone。 ２回目以降は区切らない
                        tokens = line.split(None, 2)
                        # ["#define","CMT_CH","_CMT0_CMT0"]に区切られるので要素1、2を取り出す
                        if len(tokens) == 3:
                            define_name = tokens[1]
                            #空白と（）を削除
                            define_value = clean_value(tokens[2])
                            # マクロ定義があれば値を置き換え（値が文字列の場合に数値変換する必要があるため）
                            for n in conv_macro_values:
                                # 辞書リストに一致するマクロがあるかチェック
                                #print("debug define name:"+define_name)
                                if n.get(define_value) != None:
                                    define_value = n[define_value]
                                    #print("hit:"+ define_name)
                            # scの.scfgファイルから一致するDefine名を参照
                            for key, xml_value in grid_item_dict.items():
                                description, grid_item_id = key
                                if define_name == grid_item_id:
                                     # 前後の空白を除去した値を代入
                                    xml_value_cleaned = clean_value(xml_value)
                                    # 10進数に変換してから比較
                                    # 数値データか？（True）、文字データか？（Faluse）
                                    if is_hexadecimal_or_string(xml_value_cleaned) == True:
                                        xml_value_decimal = int(convert_to_decimal(xml_value_cleaned))
                                    else:
                                        xml_value_decimal = xml_value_cleaned

                                    # 数値データか？（True）、文字データか？（Faluse）
                                    if is_hexadecimal_or_string(define_value) == True:
                                        define_value_decimal = int(convert_to_decimal(define_value))
                                    else:
                                        define_value_decimal = define_value

                                    if xml_value_decimal != define_value_decimal:
                                        print(f"Error: {define_name} in {description} does not match XML value. XML: {xml_value_cleaned}, Header: {define_value}")
                                        warning_count += 1

# frtos_configフォルダ内のFreeRTOSConfig.hを読み込む
frtos_config_path = ".//Projects//aws_ryz014a_ck_rx65n//e2studio_ccrx//src//frtos_config//FreeRTOSConfig.h"
with open(frtos_config_path, "r") as file:
    frtos_contents = file.read()
    for line in frtos_contents.split("\n"):
        # コメントを削除
        line = line.split("/*")[0]
        line = line.split("//")[0]

        if line.startswith("#define"):
            tokens = line.split(None, 2)
            if len(tokens) == 3:
                define_name = tokens[1]
                define_value = clean_value(tokens[2])
                # マクロ定義があれば値を置き換え（値が文字列の場合に数値変換する必要があるため）
                for n in conv_macro_values:
                    # 辞書リストに一致するマクロがあるかチェック
                    #print("debug define name:"+define_name)
                    if n.get(define_value) != None:
                        define_value = n[define_value]                
                for key, xml_value in grid_item_dict.items():
                    description, grid_item_id = key
                    if define_name == grid_item_id:
                        xml_value_cleaned = clean_value(xml_value)
                        # 10進数に変換してから比較
                        try:
                            # 10進数に変換してから比較
                            # 数値データか？（True）、文字データか？（Faluse）
                            if is_hexadecimal_or_string(xml_value_cleaned) == True:
                                xml_value_decimal = int(convert_to_decimal(xml_value_cleaned))
                            else:
                                xml_value_decimal = xml_value_cleaned
                            # 数値データか？（True）、文字データか？（Faluse）
                            if is_hexadecimal_or_string(define_value) == True:
                                define_value_decimal = int(convert_to_decimal(define_value))
                            else:
                                define_value_decimal = define_value

                            if xml_value_decimal != define_value_decimal:
                                print(f"Warning: {define_name} in {description} does not match XML value (numerical comparison). XML: {xml_value_cleaned}, FreeRTOSConfig.h: {define_value}")
                                warning_count += 1
                        except ValueError:
                            print(f"Error: {define_name} in {description} does not match XML value. XML: {xml_value_cleaned}, FreeRTOSConfig.h: {define_value}")
                            error_count += 1

if error_count == 0 and warning_count == 0:
    print("No errors or warnings found.")
