	.cpu cortex-m0plus
	.arch armv6s-m
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 38, 1
	.eabi_attribute 18, 4
	.file	"lv_draw_dma2d.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 1 "E:\\GIT-Repos\\RaspiPico\\DualCore\\lvgl\\src\\draw\\dma2d\\lv_draw_dma2d.c"
.Letext0:
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x1d
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF822
	.byte	0xc
	.4byte	.LASF823
	.4byte	.LASF824
	.4byte	.Ldebug_line0
	.4byte	.Ldebug_macro0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x2134
	.uleb128 0x19
	.uleb128 0x10
	.uleb128 0x17
	.uleb128 0x2119
	.uleb128 0x17
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_pubnames,"",%progbits
	.4byte	0xe
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x21
	.4byte	0
	.section	.debug_pubtypes,"",%progbits
	.4byte	0xe
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x21
	.4byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x14
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	0
	.4byte	0
	.section	.debug_macro,"",%progbits
.Ldebug_macro0:
	.2byte	0x4
	.byte	0x2
	.4byte	.Ldebug_line0
	.byte	0x7
	.4byte	.Ldebug_macro2
	.byte	0x3
	.uleb128 0
	.uleb128 0x1
	.file 2 "E:\\GIT-Repos\\RaspiPico\\DualCore\\lvgl\\src\\draw\\dma2d\\lv_draw_dma2d_private.h"
	.byte	0x3
	.uleb128 0xa
	.uleb128 0x2
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF491
	.file 3 "E:\\GIT-Repos\\RaspiPico\\DualCore\\lvgl\\src\\draw\\dma2d\\lv_draw_dma2d.h"
	.byte	0x3
	.uleb128 0x11
	.uleb128 0x3
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF492
	.file 4 "E:/GIT-Repos/RaspiPico/DualCore/lvgl/src/lv_conf_internal.h"
	.byte	0x3
	.uleb128 0x11
	.uleb128 0x4
	.byte	0x7
	.4byte	.Ldebug_macro3
	.file 5 "E:/GIT-Repos/RaspiPico/DualCore/lvgl/src/lv_conf_kconfig.h"
	.byte	0x3
	.uleb128 0x26
	.uleb128 0x5
	.byte	0x5
	.uleb128 0x4
	.4byte	.LASF514
	.byte	0x4
	.file 6 "E:/GIT-Repos/RaspiPico/DualCore/lvgl/lv_conf.h"
	.byte	0x3
	.uleb128 0x3c
	.uleb128 0x6
	.byte	0x7
	.4byte	.Ldebug_macro4
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro5
	.byte	0x4
	.byte	0x4
	.byte	0x4
	.byte	0x4
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.0.b97d5a6dff1c68edba5704b2c2d1c5e8,comdat
.Ldebug_macro2:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0
	.4byte	.LASF0
	.byte	0x5
	.uleb128 0
	.4byte	.LASF1
	.byte	0x5
	.uleb128 0
	.4byte	.LASF2
	.byte	0x5
	.uleb128 0
	.4byte	.LASF3
	.byte	0x5
	.uleb128 0
	.4byte	.LASF4
	.byte	0x5
	.uleb128 0
	.4byte	.LASF5
	.byte	0x5
	.uleb128 0
	.4byte	.LASF6
	.byte	0x5
	.uleb128 0
	.4byte	.LASF7
	.byte	0x5
	.uleb128 0
	.4byte	.LASF8
	.byte	0x5
	.uleb128 0
	.4byte	.LASF9
	.byte	0x5
	.uleb128 0
	.4byte	.LASF10
	.byte	0x5
	.uleb128 0
	.4byte	.LASF11
	.byte	0x5
	.uleb128 0
	.4byte	.LASF12
	.byte	0x5
	.uleb128 0
	.4byte	.LASF13
	.byte	0x5
	.uleb128 0
	.4byte	.LASF14
	.byte	0x5
	.uleb128 0
	.4byte	.LASF15
	.byte	0x5
	.uleb128 0
	.4byte	.LASF16
	.byte	0x5
	.uleb128 0
	.4byte	.LASF17
	.byte	0x5
	.uleb128 0
	.4byte	.LASF18
	.byte	0x5
	.uleb128 0
	.4byte	.LASF19
	.byte	0x5
	.uleb128 0
	.4byte	.LASF20
	.byte	0x5
	.uleb128 0
	.4byte	.LASF21
	.byte	0x5
	.uleb128 0
	.4byte	.LASF22
	.byte	0x5
	.uleb128 0
	.4byte	.LASF23
	.byte	0x5
	.uleb128 0
	.4byte	.LASF24
	.byte	0x5
	.uleb128 0
	.4byte	.LASF25
	.byte	0x5
	.uleb128 0
	.4byte	.LASF26
	.byte	0x5
	.uleb128 0
	.4byte	.LASF27
	.byte	0x5
	.uleb128 0
	.4byte	.LASF28
	.byte	0x5
	.uleb128 0
	.4byte	.LASF29
	.byte	0x5
	.uleb128 0
	.4byte	.LASF30
	.byte	0x5
	.uleb128 0
	.4byte	.LASF31
	.byte	0x5
	.uleb128 0
	.4byte	.LASF32
	.byte	0x5
	.uleb128 0
	.4byte	.LASF33
	.byte	0x5
	.uleb128 0
	.4byte	.LASF34
	.byte	0x5
	.uleb128 0
	.4byte	.LASF35
	.byte	0x5
	.uleb128 0
	.4byte	.LASF36
	.byte	0x5
	.uleb128 0
	.4byte	.LASF37
	.byte	0x5
	.uleb128 0
	.4byte	.LASF38
	.byte	0x5
	.uleb128 0
	.4byte	.LASF39
	.byte	0x5
	.uleb128 0
	.4byte	.LASF40
	.byte	0x5
	.uleb128 0
	.4byte	.LASF41
	.byte	0x5
	.uleb128 0
	.4byte	.LASF42
	.byte	0x5
	.uleb128 0
	.4byte	.LASF43
	.byte	0x5
	.uleb128 0
	.4byte	.LASF44
	.byte	0x5
	.uleb128 0
	.4byte	.LASF45
	.byte	0x5
	.uleb128 0
	.4byte	.LASF46
	.byte	0x5
	.uleb128 0
	.4byte	.LASF47
	.byte	0x5
	.uleb128 0
	.4byte	.LASF48
	.byte	0x5
	.uleb128 0
	.4byte	.LASF49
	.byte	0x5
	.uleb128 0
	.4byte	.LASF50
	.byte	0x5
	.uleb128 0
	.4byte	.LASF51
	.byte	0x5
	.uleb128 0
	.4byte	.LASF52
	.byte	0x5
	.uleb128 0
	.4byte	.LASF53
	.byte	0x5
	.uleb128 0
	.4byte	.LASF54
	.byte	0x5
	.uleb128 0
	.4byte	.LASF55
	.byte	0x5
	.uleb128 0
	.4byte	.LASF56
	.byte	0x5
	.uleb128 0
	.4byte	.LASF57
	.byte	0x5
	.uleb128 0
	.4byte	.LASF58
	.byte	0x5
	.uleb128 0
	.4byte	.LASF59
	.byte	0x5
	.uleb128 0
	.4byte	.LASF60
	.byte	0x5
	.uleb128 0
	.4byte	.LASF61
	.byte	0x5
	.uleb128 0
	.4byte	.LASF62
	.byte	0x5
	.uleb128 0
	.4byte	.LASF63
	.byte	0x5
	.uleb128 0
	.4byte	.LASF64
	.byte	0x5
	.uleb128 0
	.4byte	.LASF65
	.byte	0x5
	.uleb128 0
	.4byte	.LASF66
	.byte	0x5
	.uleb128 0
	.4byte	.LASF67
	.byte	0x5
	.uleb128 0
	.4byte	.LASF68
	.byte	0x5
	.uleb128 0
	.4byte	.LASF69
	.byte	0x5
	.uleb128 0
	.4byte	.LASF70
	.byte	0x5
	.uleb128 0
	.4byte	.LASF71
	.byte	0x5
	.uleb128 0
	.4byte	.LASF72
	.byte	0x5
	.uleb128 0
	.4byte	.LASF73
	.byte	0x5
	.uleb128 0
	.4byte	.LASF74
	.byte	0x5
	.uleb128 0
	.4byte	.LASF75
	.byte	0x5
	.uleb128 0
	.4byte	.LASF76
	.byte	0x5
	.uleb128 0
	.4byte	.LASF77
	.byte	0x5
	.uleb128 0
	.4byte	.LASF78
	.byte	0x5
	.uleb128 0
	.4byte	.LASF79
	.byte	0x5
	.uleb128 0
	.4byte	.LASF80
	.byte	0x5
	.uleb128 0
	.4byte	.LASF81
	.byte	0x5
	.uleb128 0
	.4byte	.LASF82
	.byte	0x5
	.uleb128 0
	.4byte	.LASF83
	.byte	0x5
	.uleb128 0
	.4byte	.LASF84
	.byte	0x5
	.uleb128 0
	.4byte	.LASF85
	.byte	0x5
	.uleb128 0
	.4byte	.LASF86
	.byte	0x5
	.uleb128 0
	.4byte	.LASF87
	.byte	0x5
	.uleb128 0
	.4byte	.LASF88
	.byte	0x5
	.uleb128 0
	.4byte	.LASF89
	.byte	0x5
	.uleb128 0
	.4byte	.LASF90
	.byte	0x5
	.uleb128 0
	.4byte	.LASF91
	.byte	0x5
	.uleb128 0
	.4byte	.LASF92
	.byte	0x5
	.uleb128 0
	.4byte	.LASF93
	.byte	0x5
	.uleb128 0
	.4byte	.LASF94
	.byte	0x5
	.uleb128 0
	.4byte	.LASF95
	.byte	0x5
	.uleb128 0
	.4byte	.LASF96
	.byte	0x5
	.uleb128 0
	.4byte	.LASF97
	.byte	0x5
	.uleb128 0
	.4byte	.LASF98
	.byte	0x5
	.uleb128 0
	.4byte	.LASF99
	.byte	0x5
	.uleb128 0
	.4byte	.LASF100
	.byte	0x5
	.uleb128 0
	.4byte	.LASF101
	.byte	0x5
	.uleb128 0
	.4byte	.LASF102
	.byte	0x5
	.uleb128 0
	.4byte	.LASF103
	.byte	0x5
	.uleb128 0
	.4byte	.LASF104
	.byte	0x5
	.uleb128 0
	.4byte	.LASF105
	.byte	0x5
	.uleb128 0
	.4byte	.LASF106
	.byte	0x5
	.uleb128 0
	.4byte	.LASF107
	.byte	0x5
	.uleb128 0
	.4byte	.LASF108
	.byte	0x5
	.uleb128 0
	.4byte	.LASF109
	.byte	0x5
	.uleb128 0
	.4byte	.LASF110
	.byte	0x5
	.uleb128 0
	.4byte	.LASF111
	.byte	0x5
	.uleb128 0
	.4byte	.LASF112
	.byte	0x5
	.uleb128 0
	.4byte	.LASF113
	.byte	0x5
	.uleb128 0
	.4byte	.LASF114
	.byte	0x5
	.uleb128 0
	.4byte	.LASF115
	.byte	0x5
	.uleb128 0
	.4byte	.LASF116
	.byte	0x5
	.uleb128 0
	.4byte	.LASF117
	.byte	0x5
	.uleb128 0
	.4byte	.LASF118
	.byte	0x5
	.uleb128 0
	.4byte	.LASF119
	.byte	0x5
	.uleb128 0
	.4byte	.LASF120
	.byte	0x5
	.uleb128 0
	.4byte	.LASF121
	.byte	0x5
	.uleb128 0
	.4byte	.LASF122
	.byte	0x5
	.uleb128 0
	.4byte	.LASF123
	.byte	0x5
	.uleb128 0
	.4byte	.LASF124
	.byte	0x5
	.uleb128 0
	.4byte	.LASF125
	.byte	0x5
	.uleb128 0
	.4byte	.LASF126
	.byte	0x5
	.uleb128 0
	.4byte	.LASF127
	.byte	0x5
	.uleb128 0
	.4byte	.LASF128
	.byte	0x5
	.uleb128 0
	.4byte	.LASF129
	.byte	0x5
	.uleb128 0
	.4byte	.LASF130
	.byte	0x5
	.uleb128 0
	.4byte	.LASF131
	.byte	0x5
	.uleb128 0
	.4byte	.LASF132
	.byte	0x5
	.uleb128 0
	.4byte	.LASF133
	.byte	0x5
	.uleb128 0
	.4byte	.LASF134
	.byte	0x5
	.uleb128 0
	.4byte	.LASF135
	.byte	0x5
	.uleb128 0
	.4byte	.LASF136
	.byte	0x5
	.uleb128 0
	.4byte	.LASF137
	.byte	0x5
	.uleb128 0
	.4byte	.LASF138
	.byte	0x5
	.uleb128 0
	.4byte	.LASF139
	.byte	0x5
	.uleb128 0
	.4byte	.LASF140
	.byte	0x5
	.uleb128 0
	.4byte	.LASF141
	.byte	0x5
	.uleb128 0
	.4byte	.LASF142
	.byte	0x5
	.uleb128 0
	.4byte	.LASF143
	.byte	0x5
	.uleb128 0
	.4byte	.LASF144
	.byte	0x5
	.uleb128 0
	.4byte	.LASF145
	.byte	0x5
	.uleb128 0
	.4byte	.LASF146
	.byte	0x5
	.uleb128 0
	.4byte	.LASF147
	.byte	0x5
	.uleb128 0
	.4byte	.LASF148
	.byte	0x5
	.uleb128 0
	.4byte	.LASF149
	.byte	0x5
	.uleb128 0
	.4byte	.LASF150
	.byte	0x5
	.uleb128 0
	.4byte	.LASF151
	.byte	0x5
	.uleb128 0
	.4byte	.LASF152
	.byte	0x5
	.uleb128 0
	.4byte	.LASF153
	.byte	0x5
	.uleb128 0
	.4byte	.LASF154
	.byte	0x5
	.uleb128 0
	.4byte	.LASF155
	.byte	0x5
	.uleb128 0
	.4byte	.LASF156
	.byte	0x5
	.uleb128 0
	.4byte	.LASF157
	.byte	0x5
	.uleb128 0
	.4byte	.LASF158
	.byte	0x5
	.uleb128 0
	.4byte	.LASF159
	.byte	0x5
	.uleb128 0
	.4byte	.LASF160
	.byte	0x5
	.uleb128 0
	.4byte	.LASF161
	.byte	0x5
	.uleb128 0
	.4byte	.LASF162
	.byte	0x5
	.uleb128 0
	.4byte	.LASF163
	.byte	0x5
	.uleb128 0
	.4byte	.LASF164
	.byte	0x5
	.uleb128 0
	.4byte	.LASF165
	.byte	0x5
	.uleb128 0
	.4byte	.LASF166
	.byte	0x5
	.uleb128 0
	.4byte	.LASF167
	.byte	0x5
	.uleb128 0
	.4byte	.LASF168
	.byte	0x5
	.uleb128 0
	.4byte	.LASF169
	.byte	0x5
	.uleb128 0
	.4byte	.LASF170
	.byte	0x5
	.uleb128 0
	.4byte	.LASF171
	.byte	0x5
	.uleb128 0
	.4byte	.LASF172
	.byte	0x5
	.uleb128 0
	.4byte	.LASF173
	.byte	0x5
	.uleb128 0
	.4byte	.LASF174
	.byte	0x5
	.uleb128 0
	.4byte	.LASF175
	.byte	0x5
	.uleb128 0
	.4byte	.LASF176
	.byte	0x5
	.uleb128 0
	.4byte	.LASF177
	.byte	0x5
	.uleb128 0
	.4byte	.LASF178
	.byte	0x5
	.uleb128 0
	.4byte	.LASF179
	.byte	0x5
	.uleb128 0
	.4byte	.LASF180
	.byte	0x5
	.uleb128 0
	.4byte	.LASF181
	.byte	0x5
	.uleb128 0
	.4byte	.LASF182
	.byte	0x5
	.uleb128 0
	.4byte	.LASF183
	.byte	0x5
	.uleb128 0
	.4byte	.LASF184
	.byte	0x5
	.uleb128 0
	.4byte	.LASF185
	.byte	0x5
	.uleb128 0
	.4byte	.LASF186
	.byte	0x5
	.uleb128 0
	.4byte	.LASF187
	.byte	0x5
	.uleb128 0
	.4byte	.LASF188
	.byte	0x5
	.uleb128 0
	.4byte	.LASF189
	.byte	0x5
	.uleb128 0
	.4byte	.LASF190
	.byte	0x5
	.uleb128 0
	.4byte	.LASF191
	.byte	0x5
	.uleb128 0
	.4byte	.LASF192
	.byte	0x5
	.uleb128 0
	.4byte	.LASF193
	.byte	0x5
	.uleb128 0
	.4byte	.LASF194
	.byte	0x5
	.uleb128 0
	.4byte	.LASF195
	.byte	0x5
	.uleb128 0
	.4byte	.LASF196
	.byte	0x5
	.uleb128 0
	.4byte	.LASF197
	.byte	0x5
	.uleb128 0
	.4byte	.LASF198
	.byte	0x5
	.uleb128 0
	.4byte	.LASF199
	.byte	0x5
	.uleb128 0
	.4byte	.LASF200
	.byte	0x5
	.uleb128 0
	.4byte	.LASF201
	.byte	0x5
	.uleb128 0
	.4byte	.LASF202
	.byte	0x5
	.uleb128 0
	.4byte	.LASF203
	.byte	0x5
	.uleb128 0
	.4byte	.LASF204
	.byte	0x5
	.uleb128 0
	.4byte	.LASF205
	.byte	0x5
	.uleb128 0
	.4byte	.LASF206
	.byte	0x5
	.uleb128 0
	.4byte	.LASF207
	.byte	0x5
	.uleb128 0
	.4byte	.LASF208
	.byte	0x5
	.uleb128 0
	.4byte	.LASF209
	.byte	0x5
	.uleb128 0
	.4byte	.LASF210
	.byte	0x5
	.uleb128 0
	.4byte	.LASF211
	.byte	0x5
	.uleb128 0
	.4byte	.LASF212
	.byte	0x5
	.uleb128 0
	.4byte	.LASF213
	.byte	0x5
	.uleb128 0
	.4byte	.LASF214
	.byte	0x5
	.uleb128 0
	.4byte	.LASF215
	.byte	0x5
	.uleb128 0
	.4byte	.LASF216
	.byte	0x5
	.uleb128 0
	.4byte	.LASF217
	.byte	0x5
	.uleb128 0
	.4byte	.LASF218
	.byte	0x5
	.uleb128 0
	.4byte	.LASF219
	.byte	0x5
	.uleb128 0
	.4byte	.LASF220
	.byte	0x5
	.uleb128 0
	.4byte	.LASF221
	.byte	0x5
	.uleb128 0
	.4byte	.LASF222
	.byte	0x5
	.uleb128 0
	.4byte	.LASF223
	.byte	0x5
	.uleb128 0
	.4byte	.LASF224
	.byte	0x5
	.uleb128 0
	.4byte	.LASF225
	.byte	0x5
	.uleb128 0
	.4byte	.LASF226
	.byte	0x5
	.uleb128 0
	.4byte	.LASF227
	.byte	0x5
	.uleb128 0
	.4byte	.LASF228
	.byte	0x5
	.uleb128 0
	.4byte	.LASF229
	.byte	0x5
	.uleb128 0
	.4byte	.LASF230
	.byte	0x5
	.uleb128 0
	.4byte	.LASF231
	.byte	0x5
	.uleb128 0
	.4byte	.LASF232
	.byte	0x5
	.uleb128 0
	.4byte	.LASF233
	.byte	0x5
	.uleb128 0
	.4byte	.LASF234
	.byte	0x5
	.uleb128 0
	.4byte	.LASF235
	.byte	0x5
	.uleb128 0
	.4byte	.LASF236
	.byte	0x5
	.uleb128 0
	.4byte	.LASF237
	.byte	0x5
	.uleb128 0
	.4byte	.LASF238
	.byte	0x5
	.uleb128 0
	.4byte	.LASF239
	.byte	0x5
	.uleb128 0
	.4byte	.LASF240
	.byte	0x5
	.uleb128 0
	.4byte	.LASF241
	.byte	0x5
	.uleb128 0
	.4byte	.LASF242
	.byte	0x5
	.uleb128 0
	.4byte	.LASF243
	.byte	0x5
	.uleb128 0
	.4byte	.LASF244
	.byte	0x5
	.uleb128 0
	.4byte	.LASF245
	.byte	0x5
	.uleb128 0
	.4byte	.LASF246
	.byte	0x5
	.uleb128 0
	.4byte	.LASF247
	.byte	0x5
	.uleb128 0
	.4byte	.LASF248
	.byte	0x5
	.uleb128 0
	.4byte	.LASF249
	.byte	0x5
	.uleb128 0
	.4byte	.LASF250
	.byte	0x5
	.uleb128 0
	.4byte	.LASF251
	.byte	0x5
	.uleb128 0
	.4byte	.LASF252
	.byte	0x5
	.uleb128 0
	.4byte	.LASF253
	.byte	0x5
	.uleb128 0
	.4byte	.LASF254
	.byte	0x5
	.uleb128 0
	.4byte	.LASF255
	.byte	0x5
	.uleb128 0
	.4byte	.LASF256
	.byte	0x5
	.uleb128 0
	.4byte	.LASF257
	.byte	0x5
	.uleb128 0
	.4byte	.LASF258
	.byte	0x5
	.uleb128 0
	.4byte	.LASF259
	.byte	0x5
	.uleb128 0
	.4byte	.LASF260
	.byte	0x5
	.uleb128 0
	.4byte	.LASF261
	.byte	0x5
	.uleb128 0
	.4byte	.LASF262
	.byte	0x5
	.uleb128 0
	.4byte	.LASF263
	.byte	0x5
	.uleb128 0
	.4byte	.LASF264
	.byte	0x5
	.uleb128 0
	.4byte	.LASF265
	.byte	0x5
	.uleb128 0
	.4byte	.LASF266
	.byte	0x5
	.uleb128 0
	.4byte	.LASF267
	.byte	0x5
	.uleb128 0
	.4byte	.LASF268
	.byte	0x5
	.uleb128 0
	.4byte	.LASF269
	.byte	0x5
	.uleb128 0
	.4byte	.LASF270
	.byte	0x5
	.uleb128 0
	.4byte	.LASF271
	.byte	0x5
	.uleb128 0
	.4byte	.LASF272
	.byte	0x5
	.uleb128 0
	.4byte	.LASF273
	.byte	0x5
	.uleb128 0
	.4byte	.LASF274
	.byte	0x5
	.uleb128 0
	.4byte	.LASF275
	.byte	0x5
	.uleb128 0
	.4byte	.LASF276
	.byte	0x5
	.uleb128 0
	.4byte	.LASF277
	.byte	0x5
	.uleb128 0
	.4byte	.LASF278
	.byte	0x5
	.uleb128 0
	.4byte	.LASF279
	.byte	0x5
	.uleb128 0
	.4byte	.LASF280
	.byte	0x5
	.uleb128 0
	.4byte	.LASF281
	.byte	0x5
	.uleb128 0
	.4byte	.LASF282
	.byte	0x5
	.uleb128 0
	.4byte	.LASF283
	.byte	0x5
	.uleb128 0
	.4byte	.LASF284
	.byte	0x5
	.uleb128 0
	.4byte	.LASF285
	.byte	0x5
	.uleb128 0
	.4byte	.LASF286
	.byte	0x5
	.uleb128 0
	.4byte	.LASF287
	.byte	0x5
	.uleb128 0
	.4byte	.LASF288
	.byte	0x5
	.uleb128 0
	.4byte	.LASF289
	.byte	0x5
	.uleb128 0
	.4byte	.LASF290
	.byte	0x5
	.uleb128 0
	.4byte	.LASF291
	.byte	0x5
	.uleb128 0
	.4byte	.LASF292
	.byte	0x5
	.uleb128 0
	.4byte	.LASF293
	.byte	0x5
	.uleb128 0
	.4byte	.LASF294
	.byte	0x5
	.uleb128 0
	.4byte	.LASF295
	.byte	0x5
	.uleb128 0
	.4byte	.LASF296
	.byte	0x5
	.uleb128 0
	.4byte	.LASF297
	.byte	0x5
	.uleb128 0
	.4byte	.LASF298
	.byte	0x5
	.uleb128 0
	.4byte	.LASF299
	.byte	0x5
	.uleb128 0
	.4byte	.LASF300
	.byte	0x5
	.uleb128 0
	.4byte	.LASF301
	.byte	0x5
	.uleb128 0
	.4byte	.LASF302
	.byte	0x5
	.uleb128 0
	.4byte	.LASF303
	.byte	0x5
	.uleb128 0
	.4byte	.LASF304
	.byte	0x5
	.uleb128 0
	.4byte	.LASF305
	.byte	0x5
	.uleb128 0
	.4byte	.LASF306
	.byte	0x5
	.uleb128 0
	.4byte	.LASF307
	.byte	0x5
	.uleb128 0
	.4byte	.LASF308
	.byte	0x5
	.uleb128 0
	.4byte	.LASF309
	.byte	0x5
	.uleb128 0
	.4byte	.LASF310
	.byte	0x5
	.uleb128 0
	.4byte	.LASF311
	.byte	0x5
	.uleb128 0
	.4byte	.LASF312
	.byte	0x5
	.uleb128 0
	.4byte	.LASF313
	.byte	0x5
	.uleb128 0
	.4byte	.LASF314
	.byte	0x5
	.uleb128 0
	.4byte	.LASF315
	.byte	0x5
	.uleb128 0
	.4byte	.LASF316
	.byte	0x5
	.uleb128 0
	.4byte	.LASF317
	.byte	0x5
	.uleb128 0
	.4byte	.LASF318
	.byte	0x5
	.uleb128 0
	.4byte	.LASF319
	.byte	0x5
	.uleb128 0
	.4byte	.LASF320
	.byte	0x5
	.uleb128 0
	.4byte	.LASF321
	.byte	0x5
	.uleb128 0
	.4byte	.LASF322
	.byte	0x5
	.uleb128 0
	.4byte	.LASF323
	.byte	0x5
	.uleb128 0
	.4byte	.LASF324
	.byte	0x5
	.uleb128 0
	.4byte	.LASF325
	.byte	0x5
	.uleb128 0
	.4byte	.LASF326
	.byte	0x5
	.uleb128 0
	.4byte	.LASF327
	.byte	0x5
	.uleb128 0
	.4byte	.LASF328
	.byte	0x5
	.uleb128 0
	.4byte	.LASF329
	.byte	0x5
	.uleb128 0
	.4byte	.LASF330
	.byte	0x5
	.uleb128 0
	.4byte	.LASF331
	.byte	0x5
	.uleb128 0
	.4byte	.LASF332
	.byte	0x5
	.uleb128 0
	.4byte	.LASF333
	.byte	0x5
	.uleb128 0
	.4byte	.LASF334
	.byte	0x5
	.uleb128 0
	.4byte	.LASF335
	.byte	0x5
	.uleb128 0
	.4byte	.LASF336
	.byte	0x5
	.uleb128 0
	.4byte	.LASF337
	.byte	0x5
	.uleb128 0
	.4byte	.LASF338
	.byte	0x5
	.uleb128 0
	.4byte	.LASF339
	.byte	0x5
	.uleb128 0
	.4byte	.LASF340
	.byte	0x5
	.uleb128 0
	.4byte	.LASF341
	.byte	0x5
	.uleb128 0
	.4byte	.LASF342
	.byte	0x5
	.uleb128 0
	.4byte	.LASF343
	.byte	0x5
	.uleb128 0
	.4byte	.LASF344
	.byte	0x5
	.uleb128 0
	.4byte	.LASF345
	.byte	0x5
	.uleb128 0
	.4byte	.LASF346
	.byte	0x5
	.uleb128 0
	.4byte	.LASF347
	.byte	0x5
	.uleb128 0
	.4byte	.LASF348
	.byte	0x5
	.uleb128 0
	.4byte	.LASF349
	.byte	0x5
	.uleb128 0
	.4byte	.LASF350
	.byte	0x5
	.uleb128 0
	.4byte	.LASF351
	.byte	0x5
	.uleb128 0
	.4byte	.LASF352
	.byte	0x5
	.uleb128 0
	.4byte	.LASF353
	.byte	0x5
	.uleb128 0
	.4byte	.LASF354
	.byte	0x5
	.uleb128 0
	.4byte	.LASF355
	.byte	0x5
	.uleb128 0
	.4byte	.LASF356
	.byte	0x5
	.uleb128 0
	.4byte	.LASF357
	.byte	0x5
	.uleb128 0
	.4byte	.LASF358
	.byte	0x5
	.uleb128 0
	.4byte	.LASF359
	.byte	0x5
	.uleb128 0
	.4byte	.LASF360
	.byte	0x5
	.uleb128 0
	.4byte	.LASF361
	.byte	0x5
	.uleb128 0
	.4byte	.LASF362
	.byte	0x5
	.uleb128 0
	.4byte	.LASF363
	.byte	0x5
	.uleb128 0
	.4byte	.LASF364
	.byte	0x5
	.uleb128 0
	.4byte	.LASF365
	.byte	0x5
	.uleb128 0
	.4byte	.LASF366
	.byte	0x5
	.uleb128 0
	.4byte	.LASF367
	.byte	0x5
	.uleb128 0
	.4byte	.LASF368
	.byte	0x5
	.uleb128 0
	.4byte	.LASF369
	.byte	0x5
	.uleb128 0
	.4byte	.LASF370
	.byte	0x5
	.uleb128 0
	.4byte	.LASF371
	.byte	0x5
	.uleb128 0
	.4byte	.LASF372
	.byte	0x5
	.uleb128 0
	.4byte	.LASF373
	.byte	0x5
	.uleb128 0
	.4byte	.LASF374
	.byte	0x5
	.uleb128 0
	.4byte	.LASF375
	.byte	0x5
	.uleb128 0
	.4byte	.LASF376
	.byte	0x5
	.uleb128 0
	.4byte	.LASF377
	.byte	0x5
	.uleb128 0
	.4byte	.LASF378
	.byte	0x5
	.uleb128 0
	.4byte	.LASF379
	.byte	0x5
	.uleb128 0
	.4byte	.LASF380
	.byte	0x5
	.uleb128 0
	.4byte	.LASF381
	.byte	0x5
	.uleb128 0
	.4byte	.LASF382
	.byte	0x5
	.uleb128 0
	.4byte	.LASF383
	.byte	0x5
	.uleb128 0
	.4byte	.LASF384
	.byte	0x5
	.uleb128 0
	.4byte	.LASF385
	.byte	0x5
	.uleb128 0
	.4byte	.LASF386
	.byte	0x5
	.uleb128 0
	.4byte	.LASF387
	.byte	0x5
	.uleb128 0
	.4byte	.LASF388
	.byte	0x5
	.uleb128 0
	.4byte	.LASF389
	.byte	0x5
	.uleb128 0
	.4byte	.LASF390
	.byte	0x5
	.uleb128 0
	.4byte	.LASF391
	.byte	0x5
	.uleb128 0
	.4byte	.LASF392
	.byte	0x5
	.uleb128 0
	.4byte	.LASF393
	.byte	0x5
	.uleb128 0
	.4byte	.LASF394
	.byte	0x5
	.uleb128 0
	.4byte	.LASF395
	.byte	0x5
	.uleb128 0
	.4byte	.LASF396
	.byte	0x5
	.uleb128 0
	.4byte	.LASF397
	.byte	0x5
	.uleb128 0
	.4byte	.LASF398
	.byte	0x5
	.uleb128 0
	.4byte	.LASF399
	.byte	0x6
	.uleb128 0
	.4byte	.LASF400
	.byte	0x6
	.uleb128 0
	.4byte	.LASF401
	.byte	0x6
	.uleb128 0
	.4byte	.LASF402
	.byte	0x6
	.uleb128 0
	.4byte	.LASF403
	.byte	0x6
	.uleb128 0
	.4byte	.LASF404
	.byte	0x6
	.uleb128 0
	.4byte	.LASF405
	.byte	0x6
	.uleb128 0
	.4byte	.LASF406
	.byte	0x6
	.uleb128 0
	.4byte	.LASF407
	.byte	0x6
	.uleb128 0
	.4byte	.LASF408
	.byte	0x6
	.uleb128 0
	.4byte	.LASF409
	.byte	0x6
	.uleb128 0
	.4byte	.LASF410
	.byte	0x6
	.uleb128 0
	.4byte	.LASF411
	.byte	0x6
	.uleb128 0
	.4byte	.LASF412
	.byte	0x6
	.uleb128 0
	.4byte	.LASF413
	.byte	0x6
	.uleb128 0
	.4byte	.LASF414
	.byte	0x6
	.uleb128 0
	.4byte	.LASF415
	.byte	0x6
	.uleb128 0
	.4byte	.LASF416
	.byte	0x6
	.uleb128 0
	.4byte	.LASF417
	.byte	0x6
	.uleb128 0
	.4byte	.LASF418
	.byte	0x6
	.uleb128 0
	.4byte	.LASF419
	.byte	0x6
	.uleb128 0
	.4byte	.LASF420
	.byte	0x6
	.uleb128 0
	.4byte	.LASF421
	.byte	0x5
	.uleb128 0
	.4byte	.LASF422
	.byte	0x5
	.uleb128 0
	.4byte	.LASF423
	.byte	0x6
	.uleb128 0
	.4byte	.LASF424
	.byte	0x5
	.uleb128 0
	.4byte	.LASF425
	.byte	0x5
	.uleb128 0
	.4byte	.LASF426
	.byte	0x6
	.uleb128 0
	.4byte	.LASF427
	.byte	0x5
	.uleb128 0
	.4byte	.LASF428
	.byte	0x5
	.uleb128 0
	.4byte	.LASF429
	.byte	0x6
	.uleb128 0
	.4byte	.LASF430
	.byte	0x5
	.uleb128 0
	.4byte	.LASF431
	.byte	0x6
	.uleb128 0
	.4byte	.LASF432
	.byte	0x5
	.uleb128 0
	.4byte	.LASF433
	.byte	0x6
	.uleb128 0
	.4byte	.LASF434
	.byte	0x5
	.uleb128 0
	.4byte	.LASF435
	.byte	0x5
	.uleb128 0
	.4byte	.LASF436
	.byte	0x5
	.uleb128 0
	.4byte	.LASF437
	.byte	0x5
	.uleb128 0
	.4byte	.LASF438
	.byte	0x6
	.uleb128 0
	.4byte	.LASF439
	.byte	0x5
	.uleb128 0
	.4byte	.LASF440
	.byte	0x6
	.uleb128 0
	.4byte	.LASF441
	.byte	0x5
	.uleb128 0
	.4byte	.LASF442
	.byte	0x6
	.uleb128 0
	.4byte	.LASF443
	.byte	0x6
	.uleb128 0
	.4byte	.LASF444
	.byte	0x6
	.uleb128 0
	.4byte	.LASF445
	.byte	0x6
	.uleb128 0
	.4byte	.LASF446
	.byte	0x6
	.uleb128 0
	.4byte	.LASF447
	.byte	0x6
	.uleb128 0
	.4byte	.LASF448
	.byte	0x6
	.uleb128 0
	.4byte	.LASF449
	.byte	0x5
	.uleb128 0
	.4byte	.LASF450
	.byte	0x5
	.uleb128 0
	.4byte	.LASF451
	.byte	0x5
	.uleb128 0
	.4byte	.LASF452
	.byte	0x5
	.uleb128 0
	.4byte	.LASF453
	.byte	0x6
	.uleb128 0
	.4byte	.LASF454
	.byte	0x6
	.uleb128 0
	.4byte	.LASF455
	.byte	0x6
	.uleb128 0
	.4byte	.LASF456
	.byte	0x6
	.uleb128 0
	.4byte	.LASF457
	.byte	0x6
	.uleb128 0
	.4byte	.LASF458
	.byte	0x6
	.uleb128 0
	.4byte	.LASF459
	.byte	0x6
	.uleb128 0
	.4byte	.LASF460
	.byte	0x6
	.uleb128 0
	.4byte	.LASF461
	.byte	0x6
	.uleb128 0
	.4byte	.LASF462
	.byte	0x6
	.uleb128 0
	.4byte	.LASF463
	.byte	0x6
	.uleb128 0
	.4byte	.LASF464
	.byte	0x5
	.uleb128 0
	.4byte	.LASF465
	.byte	0x5
	.uleb128 0
	.4byte	.LASF466
	.byte	0x5
	.uleb128 0
	.4byte	.LASF451
	.byte	0x5
	.uleb128 0
	.4byte	.LASF467
	.byte	0x5
	.uleb128 0
	.4byte	.LASF468
	.byte	0x5
	.uleb128 0
	.4byte	.LASF469
	.byte	0x5
	.uleb128 0
	.4byte	.LASF470
	.byte	0x5
	.uleb128 0
	.4byte	.LASF471
	.byte	0x5
	.uleb128 0
	.4byte	.LASF472
	.byte	0x5
	.uleb128 0
	.4byte	.LASF473
	.byte	0x5
	.uleb128 0
	.4byte	.LASF474
	.byte	0x5
	.uleb128 0
	.4byte	.LASF475
	.byte	0x5
	.uleb128 0
	.4byte	.LASF476
	.byte	0x5
	.uleb128 0
	.4byte	.LASF477
	.byte	0x5
	.uleb128 0
	.4byte	.LASF478
	.byte	0x5
	.uleb128 0
	.4byte	.LASF479
	.byte	0x5
	.uleb128 0
	.4byte	.LASF480
	.byte	0x5
	.uleb128 0
	.4byte	.LASF481
	.byte	0x5
	.uleb128 0
	.4byte	.LASF482
	.byte	0x5
	.uleb128 0
	.4byte	.LASF483
	.byte	0x5
	.uleb128 0
	.4byte	.LASF484
	.byte	0x5
	.uleb128 0
	.4byte	.LASF485
	.byte	0x5
	.uleb128 0
	.4byte	.LASF486
	.byte	0x5
	.uleb128 0
	.4byte	.LASF487
	.byte	0x5
	.uleb128 0
	.4byte	.LASF488
	.byte	0x5
	.uleb128 0
	.4byte	.LASF489
	.byte	0x5
	.uleb128 0
	.4byte	.LASF490
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.lv_conf_internal.h.8.612551d09d2b6271f6afa08e4cdfbe14,comdat
.Ldebug_macro3:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF493
	.byte	0x5
	.uleb128 0xc
	.4byte	.LASF494
	.byte	0x5
	.uleb128 0xd
	.4byte	.LASF495
	.byte	0x5
	.uleb128 0xe
	.4byte	.LASF496
	.byte	0x5
	.uleb128 0xf
	.4byte	.LASF497
	.byte	0x5
	.uleb128 0x10
	.4byte	.LASF498
	.byte	0x5
	.uleb128 0x11
	.4byte	.LASF499
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF500
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF501
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF502
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF503
	.byte	0x5
	.uleb128 0x17
	.4byte	.LASF504
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF505
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF506
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF507
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF508
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF509
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF510
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF511
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF512
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF513
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.lv_conf.h.18.63af96f3f5c43bfca5cb7d4482dfe315,comdat
.Ldebug_macro4:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF515
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF516
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF517
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF518
	.byte	0x5
	.uleb128 0x3d
	.4byte	.LASF519
	.byte	0x5
	.uleb128 0x3f
	.4byte	.LASF520
	.byte	0x5
	.uleb128 0x40
	.4byte	.LASF521
	.byte	0x5
	.uleb128 0x41
	.4byte	.LASF522
	.byte	0x5
	.uleb128 0x42
	.4byte	.LASF523
	.byte	0x5
	.uleb128 0x43
	.4byte	.LASF524
	.byte	0x5
	.uleb128 0x44
	.4byte	.LASF525
	.byte	0x5
	.uleb128 0x48
	.4byte	.LASF526
	.byte	0x5
	.uleb128 0x4b
	.4byte	.LASF527
	.byte	0x5
	.uleb128 0x4e
	.4byte	.LASF528
	.byte	0x6
	.uleb128 0x51
	.4byte	.LASF529
	.byte	0x6
	.uleb128 0x52
	.4byte	.LASF530
	.byte	0x5
	.uleb128 0x5b
	.4byte	.LASF531
	.byte	0x5
	.uleb128 0x5f
	.4byte	.LASF532
	.byte	0x5
	.uleb128 0x6e
	.4byte	.LASF533
	.byte	0x5
	.uleb128 0x81
	.4byte	.LASF534
	.byte	0x5
	.uleb128 0x84
	.4byte	.LASF535
	.byte	0x5
	.uleb128 0x8a
	.4byte	.LASF536
	.byte	0x5
	.uleb128 0x92
	.4byte	.LASF537
	.byte	0x5
	.uleb128 0x98
	.4byte	.LASF538
	.byte	0x5
	.uleb128 0x9d
	.4byte	.LASF539
	.byte	0x5
	.uleb128 0xa6
	.4byte	.LASF540
	.byte	0x5
	.uleb128 0xa8
	.4byte	.LASF541
	.byte	0x5
	.uleb128 0xb0
	.4byte	.LASF542
	.byte	0x5
	.uleb128 0xb1
	.4byte	.LASF543
	.byte	0x5
	.uleb128 0xb2
	.4byte	.LASF544
	.byte	0x5
	.uleb128 0xb3
	.4byte	.LASF545
	.byte	0x5
	.uleb128 0xb4
	.4byte	.LASF546
	.byte	0x5
	.uleb128 0xb5
	.4byte	.LASF547
	.byte	0x5
	.uleb128 0xb6
	.4byte	.LASF548
	.byte	0x5
	.uleb128 0xb7
	.4byte	.LASF549
	.byte	0x5
	.uleb128 0xb8
	.4byte	.LASF550
	.byte	0x5
	.uleb128 0xb9
	.4byte	.LASF551
	.byte	0x5
	.uleb128 0xba
	.4byte	.LASF552
	.byte	0x5
	.uleb128 0xbe
	.4byte	.LASF553
	.byte	0x5
	.uleb128 0xc3
	.4byte	.LASF554
	.byte	0x5
	.uleb128 0xc6
	.4byte	.LASF555
	.byte	0x5
	.uleb128 0xc9
	.4byte	.LASF556
	.byte	0x5
	.uleb128 0xce
	.4byte	.LASF557
	.byte	0x5
	.uleb128 0xd4
	.4byte	.LASF558
	.byte	0x5
	.uleb128 0xda
	.4byte	.LASF559
	.byte	0x5
	.uleb128 0xdd
	.4byte	.LASF560
	.byte	0x5
	.uleb128 0xe4
	.4byte	.LASF561
	.byte	0x5
	.uleb128 0xe9
	.4byte	.LASF562
	.byte	0x5
	.uleb128 0xfe
	.4byte	.LASF563
	.byte	0x5
	.uleb128 0x111
	.4byte	.LASF564
	.byte	0x5
	.uleb128 0x128
	.4byte	.LASF565
	.byte	0x5
	.uleb128 0x12b
	.4byte	.LASF566
	.byte	0x5
	.uleb128 0x12e
	.4byte	.LASF567
	.byte	0x5
	.uleb128 0x172
	.4byte	.LASF568
	.byte	0x5
	.uleb128 0x17d
	.4byte	.LASF569
	.byte	0x5
	.uleb128 0x183
	.4byte	.LASF570
	.byte	0x5
	.uleb128 0x189
	.4byte	.LASF571
	.byte	0x5
	.uleb128 0x19d
	.4byte	.LASF572
	.byte	0x5
	.uleb128 0x1cb
	.4byte	.LASF573
	.byte	0x5
	.uleb128 0x1cc
	.4byte	.LASF574
	.byte	0x5
	.uleb128 0x1cd
	.4byte	.LASF575
	.byte	0x5
	.uleb128 0x1ce
	.4byte	.LASF576
	.byte	0x5
	.uleb128 0x1cf
	.4byte	.LASF577
	.byte	0x5
	.uleb128 0x1d2
	.4byte	.LASF578
	.byte	0x5
	.uleb128 0x1d3
	.4byte	.LASF579
	.byte	0x5
	.uleb128 0x1da
	.4byte	.LASF580
	.byte	0x5
	.uleb128 0x1dd
	.4byte	.LASF581
	.byte	0x5
	.uleb128 0x1e3
	.4byte	.LASF582
	.byte	0x5
	.uleb128 0x1e9
	.4byte	.LASF583
	.byte	0x5
	.uleb128 0x1f4
	.4byte	.LASF584
	.byte	0x5
	.uleb128 0x1f8
	.4byte	.LASF585
	.byte	0x5
	.uleb128 0x1fc
	.4byte	.LASF586
	.byte	0x5
	.uleb128 0x204
	.4byte	.LASF587
	.byte	0x5
	.uleb128 0x207
	.4byte	.LASF588
	.byte	0x5
	.uleb128 0x20a
	.4byte	.LASF589
	.byte	0x5
	.uleb128 0x20d
	.4byte	.LASF590
	.byte	0x5
	.uleb128 0x210
	.4byte	.LASF591
	.byte	0x5
	.uleb128 0x218
	.4byte	.LASF592
	.byte	0x5
	.uleb128 0x21b
	.4byte	.LASF593
	.byte	0x5
	.uleb128 0x21e
	.4byte	.LASF594
	.byte	0x5
	.uleb128 0x222
	.4byte	.LASF595
	.byte	0x5
	.uleb128 0x229
	.4byte	.LASF596
	.byte	0x5
	.uleb128 0x22c
	.4byte	.LASF597
	.byte	0x5
	.uleb128 0x22f
	.4byte	.LASF598
	.byte	0x5
	.uleb128 0x232
	.4byte	.LASF599
	.byte	0x5
	.uleb128 0x236
	.4byte	.LASF600
	.byte	0x5
	.uleb128 0x23a
	.4byte	.LASF601
	.byte	0x5
	.uleb128 0x23d
	.4byte	.LASF602
	.byte	0x5
	.uleb128 0x240
	.4byte	.LASF603
	.byte	0x5
	.uleb128 0x243
	.4byte	.LASF604
	.byte	0x5
	.uleb128 0x247
	.4byte	.LASF605
	.byte	0x5
	.uleb128 0x24a
	.4byte	.LASF606
	.byte	0x5
	.uleb128 0x24d
	.4byte	.LASF607
	.byte	0x5
	.uleb128 0x251
	.4byte	.LASF608
	.byte	0x5
	.uleb128 0x255
	.4byte	.LASF609
	.byte	0x5
	.uleb128 0x25e
	.4byte	.LASF610
	.byte	0x5
	.uleb128 0x25f
	.4byte	.LASF611
	.byte	0x5
	.uleb128 0x260
	.4byte	.LASF612
	.byte	0x5
	.uleb128 0x261
	.4byte	.LASF613
	.byte	0x5
	.uleb128 0x262
	.4byte	.LASF614
	.byte	0x5
	.uleb128 0x263
	.4byte	.LASF615
	.byte	0x5
	.uleb128 0x264
	.4byte	.LASF616
	.byte	0x5
	.uleb128 0x265
	.4byte	.LASF617
	.byte	0x5
	.uleb128 0x266
	.4byte	.LASF618
	.byte	0x5
	.uleb128 0x267
	.4byte	.LASF619
	.byte	0x5
	.uleb128 0x268
	.4byte	.LASF620
	.byte	0x5
	.uleb128 0x269
	.4byte	.LASF621
	.byte	0x5
	.uleb128 0x26a
	.4byte	.LASF622
	.byte	0x5
	.uleb128 0x26b
	.4byte	.LASF623
	.byte	0x5
	.uleb128 0x26c
	.4byte	.LASF624
	.byte	0x5
	.uleb128 0x26d
	.4byte	.LASF625
	.byte	0x5
	.uleb128 0x26e
	.4byte	.LASF626
	.byte	0x5
	.uleb128 0x26f
	.4byte	.LASF627
	.byte	0x5
	.uleb128 0x270
	.4byte	.LASF628
	.byte	0x5
	.uleb128 0x271
	.4byte	.LASF629
	.byte	0x5
	.uleb128 0x272
	.4byte	.LASF630
	.byte	0x5
	.uleb128 0x275
	.4byte	.LASF631
	.byte	0x5
	.uleb128 0x276
	.4byte	.LASF632
	.byte	0x5
	.uleb128 0x277
	.4byte	.LASF633
	.byte	0x5
	.uleb128 0x278
	.4byte	.LASF634
	.byte	0x5
	.uleb128 0x27b
	.4byte	.LASF635
	.byte	0x5
	.uleb128 0x27c
	.4byte	.LASF636
	.byte	0x5
	.uleb128 0x287
	.4byte	.LASF637
	.byte	0x5
	.uleb128 0x28e
	.4byte	.LASF638
	.byte	0x5
	.uleb128 0x293
	.4byte	.LASF639
	.byte	0x5
	.uleb128 0x296
	.4byte	.LASF640
	.byte	0x5
	.uleb128 0x299
	.4byte	.LASF641
	.byte	0x5
	.uleb128 0x2a5
	.4byte	.LASF642
	.byte	0x5
	.uleb128 0x2a8
	.4byte	.LASF643
	.byte	0x5
	.uleb128 0x2ac
	.4byte	.LASF644
	.byte	0x5
	.uleb128 0x2b0
	.4byte	.LASF645
	.byte	0x5
	.uleb128 0x2b4
	.4byte	.LASF646
	.byte	0x5
	.uleb128 0x2b9
	.4byte	.LASF647
	.byte	0x5
	.uleb128 0x2c4
	.4byte	.LASF648
	.byte	0x5
	.uleb128 0x2c7
	.4byte	.LASF649
	.byte	0x5
	.uleb128 0x2d6
	.4byte	.LASF650
	.byte	0x5
	.uleb128 0x2d8
	.4byte	.LASF651
	.byte	0x5
	.uleb128 0x2da
	.4byte	.LASF652
	.byte	0x5
	.uleb128 0x2dc
	.4byte	.LASF653
	.byte	0x5
	.uleb128 0x2de
	.4byte	.LASF654
	.byte	0x5
	.uleb128 0x2e0
	.4byte	.LASF655
	.byte	0x5
	.uleb128 0x2e2
	.4byte	.LASF656
	.byte	0x5
	.uleb128 0x2e4
	.4byte	.LASF657
	.byte	0x5
	.uleb128 0x2e6
	.4byte	.LASF658
	.byte	0x5
	.uleb128 0x2ea
	.4byte	.LASF659
	.byte	0x5
	.uleb128 0x2ed
	.4byte	.LASF660
	.byte	0x5
	.uleb128 0x2ee
	.4byte	.LASF661
	.byte	0x5
	.uleb128 0x2ef
	.4byte	.LASF662
	.byte	0x5
	.uleb128 0x2f0
	.4byte	.LASF663
	.byte	0x5
	.uleb128 0x2f3
	.4byte	.LASF664
	.byte	0x5
	.uleb128 0x2f5
	.4byte	.LASF665
	.byte	0x5
	.uleb128 0x2f7
	.4byte	.LASF666
	.byte	0x5
	.uleb128 0x2f9
	.4byte	.LASF667
	.byte	0x5
	.uleb128 0x2fb
	.4byte	.LASF668
	.byte	0x5
	.uleb128 0x2fd
	.4byte	.LASF669
	.byte	0x5
	.uleb128 0x2ff
	.4byte	.LASF670
	.byte	0x5
	.uleb128 0x301
	.4byte	.LASF671
	.byte	0x5
	.uleb128 0x303
	.4byte	.LASF672
	.byte	0x5
	.uleb128 0x304
	.4byte	.LASF673
	.byte	0x5
	.uleb128 0x305
	.4byte	.LASF674
	.byte	0x5
	.uleb128 0x308
	.4byte	.LASF675
	.byte	0x5
	.uleb128 0x30a
	.4byte	.LASF676
	.byte	0x5
	.uleb128 0x30c
	.4byte	.LASF677
	.byte	0x5
	.uleb128 0x30e
	.4byte	.LASF678
	.byte	0x5
	.uleb128 0x310
	.4byte	.LASF679
	.byte	0x5
	.uleb128 0x312
	.4byte	.LASF680
	.byte	0x5
	.uleb128 0x314
	.4byte	.LASF681
	.byte	0x5
	.uleb128 0x316
	.4byte	.LASF682
	.byte	0x5
	.uleb128 0x318
	.4byte	.LASF683
	.byte	0x5
	.uleb128 0x31a
	.4byte	.LASF684
	.byte	0x5
	.uleb128 0x31d
	.4byte	.LASF685
	.byte	0x5
	.uleb128 0x320
	.4byte	.LASF686
	.byte	0x5
	.uleb128 0x322
	.4byte	.LASF687
	.byte	0x5
	.uleb128 0x324
	.4byte	.LASF688
	.byte	0x5
	.uleb128 0x326
	.4byte	.LASF689
	.byte	0x5
	.uleb128 0x328
	.4byte	.LASF690
	.byte	0x5
	.uleb128 0x32a
	.4byte	.LASF691
	.byte	0x5
	.uleb128 0x32c
	.4byte	.LASF692
	.byte	0x5
	.uleb128 0x32f
	.4byte	.LASF693
	.byte	0x5
	.uleb128 0x331
	.4byte	.LASF694
	.byte	0x5
	.uleb128 0x333
	.4byte	.LASF695
	.byte	0x5
	.uleb128 0x33b
	.4byte	.LASF696
	.byte	0x5
	.uleb128 0x33e
	.4byte	.LASF697
	.byte	0x5
	.uleb128 0x341
	.4byte	.LASF698
	.byte	0x5
	.uleb128 0x344
	.4byte	.LASF699
	.byte	0x5
	.uleb128 0x348
	.4byte	.LASF700
	.byte	0x5
	.uleb128 0x34b
	.4byte	.LASF701
	.byte	0x5
	.uleb128 0x353
	.4byte	.LASF702
	.byte	0x5
	.uleb128 0x356
	.4byte	.LASF703
	.byte	0x5
	.uleb128 0x362
	.4byte	.LASF704
	.byte	0x5
	.uleb128 0x365
	.4byte	.LASF705
	.byte	0x5
	.uleb128 0x36d
	.4byte	.LASF706
	.byte	0x5
	.uleb128 0x375
	.4byte	.LASF707
	.byte	0x5
	.uleb128 0x37d
	.4byte	.LASF708
	.byte	0x5
	.uleb128 0x385
	.4byte	.LASF709
	.byte	0x5
	.uleb128 0x38b
	.4byte	.LASF710
	.byte	0x5
	.uleb128 0x392
	.4byte	.LASF711
	.byte	0x5
	.uleb128 0x399
	.4byte	.LASF712
	.byte	0x5
	.uleb128 0x3a0
	.4byte	.LASF713
	.byte	0x5
	.uleb128 0x3a5
	.4byte	.LASF714
	.byte	0x5
	.uleb128 0x3ab
	.4byte	.LASF715
	.byte	0x5
	.uleb128 0x3ae
	.4byte	.LASF716
	.byte	0x5
	.uleb128 0x3b1
	.4byte	.LASF717
	.byte	0x5
	.uleb128 0x3b5
	.4byte	.LASF718
	.byte	0x5
	.uleb128 0x3b9
	.4byte	.LASF719
	.byte	0x5
	.uleb128 0x3bc
	.4byte	.LASF720
	.byte	0x5
	.uleb128 0x3c3
	.4byte	.LASF721
	.byte	0x5
	.uleb128 0x3c6
	.4byte	.LASF722
	.byte	0x5
	.uleb128 0x3c9
	.4byte	.LASF723
	.byte	0x5
	.uleb128 0x3cc
	.4byte	.LASF724
	.byte	0x5
	.uleb128 0x3cf
	.4byte	.LASF725
	.byte	0x5
	.uleb128 0x3d2
	.4byte	.LASF726
	.byte	0x5
	.uleb128 0x3dd
	.4byte	.LASF727
	.byte	0x5
	.uleb128 0x3e6
	.4byte	.LASF728
	.byte	0x5
	.uleb128 0x3e9
	.4byte	.LASF729
	.byte	0x5
	.uleb128 0x3ed
	.4byte	.LASF730
	.byte	0x5
	.uleb128 0x3f1
	.4byte	.LASF731
	.byte	0x5
	.uleb128 0x3f5
	.4byte	.LASF732
	.byte	0x5
	.uleb128 0x3f8
	.4byte	.LASF733
	.byte	0x5
	.uleb128 0x3fb
	.4byte	.LASF734
	.byte	0x5
	.uleb128 0x3ff
	.4byte	.LASF735
	.byte	0x5
	.uleb128 0x400
	.4byte	.LASF736
	.byte	0x5
	.uleb128 0x401
	.4byte	.LASF737
	.byte	0x5
	.uleb128 0x405
	.4byte	.LASF738
	.byte	0x5
	.uleb128 0x415
	.4byte	.LASF739
	.byte	0x5
	.uleb128 0x418
	.4byte	.LASF740
	.byte	0x5
	.uleb128 0x438
	.4byte	.LASF741
	.byte	0x5
	.uleb128 0x475
	.4byte	.LASF742
	.byte	0x5
	.uleb128 0x478
	.4byte	.LASF743
	.byte	0x5
	.uleb128 0x47b
	.4byte	.LASF744
	.byte	0x5
	.uleb128 0x47e
	.4byte	.LASF745
	.byte	0x5
	.uleb128 0x481
	.4byte	.LASF746
	.byte	0x5
	.uleb128 0x485
	.4byte	.LASF747
	.byte	0x5
	.uleb128 0x497
	.4byte	.LASF748
	.byte	0x5
	.uleb128 0x4a1
	.4byte	.LASF749
	.byte	0x5
	.uleb128 0x4aa
	.4byte	.LASF750
	.byte	0x5
	.uleb128 0x4b3
	.4byte	.LASF751
	.byte	0x5
	.uleb128 0x4b6
	.4byte	.LASF752
	.byte	0x5
	.uleb128 0x4b9
	.4byte	.LASF753
	.byte	0x5
	.uleb128 0x4c0
	.4byte	.LASF754
	.byte	0x5
	.uleb128 0x4cc
	.4byte	.LASF755
	.byte	0x5
	.uleb128 0x4d7
	.4byte	.LASF756
	.byte	0x5
	.uleb128 0x4e1
	.4byte	.LASF757
	.byte	0x5
	.uleb128 0x4eb
	.4byte	.LASF758
	.byte	0x5
	.uleb128 0x514
	.4byte	.LASF759
	.byte	0x5
	.uleb128 0x522
	.4byte	.LASF760
	.byte	0x5
	.uleb128 0x525
	.4byte	.LASF761
	.byte	0x5
	.uleb128 0x52d
	.4byte	.LASF762
	.byte	0x5
	.uleb128 0x530
	.4byte	.LASF763
	.byte	0x5
	.uleb128 0x53e
	.4byte	.LASF764
	.byte	0x5
	.uleb128 0x53f
	.4byte	.LASF765
	.byte	0x5
	.uleb128 0x540
	.4byte	.LASF766
	.byte	0x5
	.uleb128 0x541
	.4byte	.LASF767
	.byte	0x5
	.uleb128 0x542
	.4byte	.LASF768
	.byte	0x5
	.uleb128 0x543
	.4byte	.LASF769
	.byte	0x5
	.uleb128 0x545
	.4byte	.LASF770
	.byte	0x5
	.uleb128 0x548
	.4byte	.LASF771
	.byte	0x5
	.uleb128 0x551
	.4byte	.LASF772
	.byte	0x5
	.uleb128 0x555
	.4byte	.LASF773
	.byte	0x5
	.uleb128 0x558
	.4byte	.LASF774
	.byte	0x5
	.uleb128 0x55f
	.4byte	.LASF775
	.byte	0x5
	.uleb128 0x562
	.4byte	.LASF776
	.byte	0x5
	.uleb128 0x565
	.4byte	.LASF777
	.byte	0x5
	.uleb128 0x56c
	.4byte	.LASF778
	.byte	0x5
	.uleb128 0x572
	.4byte	.LASF779
	.byte	0x5
	.uleb128 0x576
	.4byte	.LASF780
	.byte	0x5
	.uleb128 0x580
	.4byte	.LASF781
	.byte	0x5
	.uleb128 0x583
	.4byte	.LASF782
	.byte	0x5
	.uleb128 0x58b
	.4byte	.LASF783
	.byte	0x5
	.uleb128 0x58e
	.4byte	.LASF784
	.byte	0x5
	.uleb128 0x591
	.4byte	.LASF785
	.byte	0x5
	.uleb128 0x59a
	.4byte	.LASF786
	.byte	0x5
	.uleb128 0x59d
	.4byte	.LASF787
	.byte	0x5
	.uleb128 0x5a0
	.4byte	.LASF788
	.byte	0x5
	.uleb128 0x5aa
	.4byte	.LASF789
	.byte	0x5
	.uleb128 0x5ad
	.4byte	.LASF790
	.byte	0x5
	.uleb128 0x5b4
	.4byte	.LASF791
	.byte	0x5
	.uleb128 0x5b7
	.4byte	.LASF792
	.byte	0x5
	.uleb128 0x5ba
	.4byte	.LASF793
	.byte	0x5
	.uleb128 0x5bd
	.4byte	.LASF794
	.byte	0x5
	.uleb128 0x5c0
	.4byte	.LASF795
	.byte	0x5
	.uleb128 0x5c6
	.4byte	.LASF796
	.byte	0x5
	.uleb128 0x5c9
	.4byte	.LASF797
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.lv_conf_internal.h.220.ef0d4bcaaa020f355fe7501bd1165e03,comdat
.Ldebug_macro5:
	.2byte	0x4
	.byte	0
	.byte	0x6
	.uleb128 0xdc
	.4byte	.LASF529
	.byte	0x6
	.uleb128 0xe3
	.4byte	.LASF530
	.byte	0x5
	.uleb128 0x1273
	.4byte	.LASF798
	.byte	0x6
	.uleb128 0x127b
	.4byte	.LASF799
	.byte	0x5
	.uleb128 0x127f
	.4byte	.LASF800
	.byte	0x5
	.uleb128 0x1283
	.4byte	.LASF801
	.byte	0x5
	.uleb128 0x1288
	.4byte	.LASF802
	.byte	0x5
	.uleb128 0x1289
	.4byte	.LASF803
	.byte	0x5
	.uleb128 0x128a
	.4byte	.LASF804
	.byte	0x5
	.uleb128 0x128b
	.4byte	.LASF805
	.byte	0x5
	.uleb128 0x128c
	.4byte	.LASF806
	.byte	0x5
	.uleb128 0x128d
	.4byte	.LASF807
	.byte	0x5
	.uleb128 0x128e
	.4byte	.LASF808
	.byte	0x5
	.uleb128 0x128f
	.4byte	.LASF809
	.byte	0x5
	.uleb128 0x1290
	.4byte	.LASF810
	.byte	0x5
	.uleb128 0x1294
	.4byte	.LASF811
	.byte	0x5
	.uleb128 0x1295
	.4byte	.LASF812
	.byte	0x5
	.uleb128 0x1299
	.4byte	.LASF813
	.byte	0x5
	.uleb128 0x129d
	.4byte	.LASF814
	.byte	0x5
	.uleb128 0x129e
	.4byte	.LASF815
	.byte	0x5
	.uleb128 0x129f
	.4byte	.LASF816
	.byte	0x5
	.uleb128 0x12a3
	.4byte	.LASF817
	.byte	0x5
	.uleb128 0x12bb
	.4byte	.LASF818
	.byte	0x5
	.uleb128 0x12c3
	.4byte	.LASF819
	.byte	0x5
	.uleb128 0x12c8
	.4byte	.LASF820
	.byte	0x5
	.uleb128 0x12d7
	.4byte	.LASF821
	.byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF238:
	.ascii	"__FLT64_EPSILON__ 1.1\000"
.LASF734:
	.ascii	"LV_USE_LZ4_EXTERNAL 0\000"
.LASF565:
	.ascii	"LV_USE_DRAW_DAVE2D 0\000"
.LASF185:
	.ascii	"__DECIMAL_DIG__ 17\000"
.LASF368:
	.ascii	"__UHA_FBIT__ 8\000"
.LASF679:
	.ascii	"LV_USE_MENU 1\000"
.LASF161:
	.ascii	"__FLT_HAS_QUIET_NAN__ 1\000"
.LASF467:
	.ascii	"__CROSSWORKS_ARM 1\000"
.LASF586:
	.ascii	"LV_GRADIENT_MAX_STOPS 2\000"
.LASF380:
	.ascii	"__CHAR_UNSIGNED__ 1\000"
.LASF617:
	.ascii	"LV_FONT_MONTSERRAT_22 0\000"
.LASF770:
	.ascii	"LV_USE_GC9A01 1\000"
.LASF241:
	.ascii	"__FLT64_HAS_INFINITY__ 1\000"
.LASF643:
	.ascii	"LV_TXT_BREAK_CHARS \" ,.;:-_)]}\"\000"
.LASF766:
	.ascii	"LV_USE_ST7796 0\000"
.LASF332:
	.ascii	"__LLACCUM_MIN__ (-0X1P31LLK-0X1P31LLK)\000"
.LASF823:
	.ascii	"E:\\GIT-Repos\\RaspiPico\\DualCore\\lvgl\\src\\draw"
	.ascii	"\\dma2d\\lv_draw_dma2d.c\000"
.LASF391:
	.ascii	"__GCC_DESTRUCTIVE_SIZE 64\000"
.LASF324:
	.ascii	"__LACCUM_EPSILON__ 0x1P-31LK\000"
.LASF79:
	.ascii	"__PTRDIFF_MAX__ 0x7fffffff\000"
.LASF702:
	.ascii	"LV_USE_FLEX 1\000"
.LASF91:
	.ascii	"__INTMAX_C(c) c ## LL\000"
.LASF237:
	.ascii	"__FLT64_MIN__ 1.1\000"
.LASF627:
	.ascii	"LV_FONT_MONTSERRAT_42 0\000"
.LASF90:
	.ascii	"__INTMAX_MAX__ 0x7fffffffffffffffLL\000"
.LASF349:
	.ascii	"__TQ_IBIT__ 0\000"
.LASF246:
	.ascii	"__FLT32X_MIN_EXP__ (-1021)\000"
.LASF14:
	.ascii	"__ATOMIC_CONSUME 1\000"
.LASF282:
	.ascii	"__LFRACT_MIN__ (-0.5LR-0.5LR)\000"
.LASF75:
	.ascii	"__WCHAR_MAX__ 0xffffffffU\000"
.LASF401:
	.ascii	"__ARM_FEATURE_QBIT\000"
.LASF18:
	.ascii	"__SIZEOF_LONG_LONG__ 8\000"
.LASF168:
	.ascii	"__DBL_MAX_10_EXP__ 308\000"
.LASF526:
	.ascii	"LV_MEM_SIZE (64 * 1024U)\000"
.LASF272:
	.ascii	"__FRACT_MIN__ (-0.5R-0.5R)\000"
.LASF495:
	.ascii	"LV_OS_PTHREAD 1\000"
.LASF338:
	.ascii	"__ULLACCUM_MAX__ 0XFFFFFFFFFFFFFFFFP-32ULLK\000"
.LASF308:
	.ascii	"__USACCUM_MAX__ 0XFFFFP-8UHK\000"
.LASF260:
	.ascii	"__SFRACT_FBIT__ 7\000"
.LASF94:
	.ascii	"__INTMAX_WIDTH__ 64\000"
.LASF414:
	.ascii	"__ARM_FEATURE_BTI_DEFAULT\000"
.LASF345:
	.ascii	"__SQ_IBIT__ 0\000"
.LASF28:
	.ascii	"__ORDER_PDP_ENDIAN__ 3412\000"
.LASF34:
	.ascii	"__SIZE_TYPE__ unsigned int\000"
.LASF718:
	.ascii	"LV_USE_TJPGD 0\000"
.LASF159:
	.ascii	"__FLT_HAS_DENORM__ 1\000"
.LASF43:
	.ascii	"__INT8_TYPE__ signed char\000"
.LASF425:
	.ascii	"__ARM_ARCH_PROFILE 77\000"
.LASF307:
	.ascii	"__USACCUM_MIN__ 0.0UHK\000"
.LASF218:
	.ascii	"__FLT32_DECIMAL_DIG__ 9\000"
.LASF666:
	.ascii	"LV_USE_CHECKBOX 1\000"
.LASF767:
	.ascii	"LV_USE_ILI9341 0\000"
.LASF181:
	.ascii	"__LDBL_MIN_EXP__ (-1021)\000"
.LASF722:
	.ascii	"LV_BIN_DECODER_RAM_LOAD 0\000"
.LASF119:
	.ascii	"__UINT8_C(c) c\000"
.LASF712:
	.ascii	"LV_USE_FS_ARDUINO_SD 0\000"
.LASF44:
	.ascii	"__INT16_TYPE__ short int\000"
.LASF497:
	.ascii	"LV_OS_CMSIS_RTOS2 3\000"
.LASF607:
	.ascii	"LV_USE_FLOAT 0\000"
.LASF247:
	.ascii	"__FLT32X_MIN_10_EXP__ (-307)\000"
.LASF662:
	.ascii	"LV_USE_CALENDAR_HEADER_DROPDOWN 1\000"
.LASF489:
	.ascii	"PICO_UART_DEFAULT_CRLF 1\000"
.LASF469:
	.ascii	"__CROSSWORKS_MAJOR_VERSION 5\000"
.LASF742:
	.ascii	"LV_USE_MONKEY 0\000"
.LASF4:
	.ascii	"__STDC_HOSTED__ 1\000"
.LASF295:
	.ascii	"__ULLFRACT_FBIT__ 64\000"
.LASF484:
	.ascii	"PICO_DEFAULT_SPI1_TX_PIN 11\000"
.LASF42:
	.ascii	"__SIG_ATOMIC_TYPE__ int\000"
.LASF62:
	.ascii	"__INT_FAST64_TYPE__ long long int\000"
.LASF69:
	.ascii	"__GXX_ABI_VERSION 1019\000"
.LASF576:
	.ascii	"LV_USE_ASSERT_MEM_INTEGRITY 0\000"
.LASF258:
	.ascii	"__FLT32X_HAS_QUIET_NAN__ 1\000"
.LASF217:
	.ascii	"__FLT32_MAX_10_EXP__ 38\000"
.LASF350:
	.ascii	"__UQQ_FBIT__ 8\000"
.LASF794:
	.ascii	"LV_USE_DEMO_SCROLL 0\000"
.LASF268:
	.ascii	"__USFRACT_MAX__ 0XFFP-8UHR\000"
.LASF419:
	.ascii	"__ARM_FEATURE_CLZ\000"
.LASF140:
	.ascii	"__UINTPTR_MAX__ 0xffffffffU\000"
.LASF656:
	.ascii	"LV_USE_BUTTONMATRIX 1\000"
.LASF214:
	.ascii	"__FLT32_MIN_EXP__ (-125)\000"
.LASF569:
	.ascii	"LV_USE_DRAW_OPENGLES 0\000"
.LASF473:
	.ascii	"__RP2040_FAMILY 1\000"
.LASF773:
	.ascii	"LV_USE_RENESAS_GLCDC 0\000"
.LASF677:
	.ascii	"LV_USE_LIST 1\000"
.LASF209:
	.ascii	"__FLT16_HAS_INFINITY__ 1\000"
.LASF285:
	.ascii	"__ULFRACT_FBIT__ 32\000"
.LASF642:
	.ascii	"LV_TXT_ENC LV_TXT_ENC_UTF8\000"
.LASF231:
	.ascii	"__FLT64_MIN_10_EXP__ (-307)\000"
.LASF32:
	.ascii	"__GNUC_EXECUTION_CHARSET_NAME \"UTF-8\"\000"
.LASF657:
	.ascii	"LV_USE_CALENDAR 1\000"
.LASF133:
	.ascii	"__INT_FAST64_WIDTH__ 64\000"
.LASF234:
	.ascii	"__FLT64_DECIMAL_DIG__ 17\000"
.LASF731:
	.ascii	"LV_USE_THORVG_INTERNAL 0\000"
.LASF264:
	.ascii	"__SFRACT_EPSILON__ 0x1P-7HR\000"
.LASF611:
	.ascii	"LV_FONT_MONTSERRAT_10 0\000"
.LASF354:
	.ascii	"__USQ_FBIT__ 32\000"
.LASF27:
	.ascii	"__ORDER_BIG_ENDIAN__ 4321\000"
.LASF68:
	.ascii	"__UINTPTR_TYPE__ unsigned int\000"
.LASF344:
	.ascii	"__SQ_FBIT__ 31\000"
.LASF235:
	.ascii	"__FLT64_MAX__ 1.1\000"
.LASF374:
	.ascii	"__UTA_FBIT__ 64\000"
.LASF758:
	.ascii	"LV_USE_NUTTX 0\000"
.LASF513:
	.ascii	"LV_NEMA_HAL_STM32 1\000"
.LASF739:
	.ascii	"LV_USE_SNAPSHOT 0\000"
.LASF710:
	.ascii	"LV_USE_FS_LITTLEFS 0\000"
.LASF352:
	.ascii	"__UHQ_FBIT__ 16\000"
.LASF486:
	.ascii	"CORE1_STACKSIZE 1024\000"
.LASF621:
	.ascii	"LV_FONT_MONTSERRAT_30 0\000"
.LASF765:
	.ascii	"LV_USE_ST7789 0\000"
.LASF88:
	.ascii	"__PTRDIFF_WIDTH__ 32\000"
.LASF254:
	.ascii	"__FLT32X_EPSILON__ 1.1\000"
.LASF777:
	.ascii	"LV_USE_UEFI 0\000"
.LASF134:
	.ascii	"__UINT_FAST8_MAX__ 0xffffffffU\000"
.LASF717:
	.ascii	"LV_USE_BMP 0\000"
.LASF154:
	.ascii	"__FLT_MAX__ 1.1\000"
.LASF321:
	.ascii	"__LACCUM_IBIT__ 32\000"
.LASF545:
	.ascii	"LV_DRAW_SW_SUPPORT_RGB888 1\000"
.LASF81:
	.ascii	"__SCHAR_WIDTH__ 8\000"
.LASF129:
	.ascii	"__INT_FAST16_WIDTH__ 32\000"
.LASF815:
	.ascii	"LV_USE_MEM_MONITOR 0\000"
.LASF438:
	.ascii	"__VFP_FP__ 1\000"
.LASF292:
	.ascii	"__LLFRACT_MIN__ (-0.5LLR-0.5LLR)\000"
.LASF138:
	.ascii	"__INTPTR_MAX__ 0x7fffffff\000"
.LASF790:
	.ascii	"LV_USE_DEMO_GLTF 0\000"
.LASF135:
	.ascii	"__UINT_FAST16_MAX__ 0xffffffffU\000"
.LASF179:
	.ascii	"__LDBL_MANT_DIG__ 53\000"
.LASF754:
	.ascii	"LV_USE_SDL 0\000"
.LASF208:
	.ascii	"__FLT16_HAS_DENORM__ 1\000"
.LASF708:
	.ascii	"LV_USE_FS_FATFS 0\000"
.LASF468:
	.ascii	"__HEAP_SIZE__ 4096\000"
.LASF413:
	.ascii	"__ARM_FEATURE_BTI\000"
.LASF277:
	.ascii	"__UFRACT_MIN__ 0.0UR\000"
.LASF521:
	.ascii	"LV_STDDEF_INCLUDE <stddef.h>\000"
.LASF194:
	.ascii	"__LDBL_HAS_QUIET_NAN__ 1\000"
.LASF748:
	.ascii	"LV_USE_FILE_EXPLORER 0\000"
.LASF257:
	.ascii	"__FLT32X_HAS_INFINITY__ 1\000"
.LASF485:
	.ascii	"PICO_DEFAULT_SPI1_RESET_PIN 12\000"
.LASF477:
	.ascii	"DEBUG 1\000"
.LASF648:
	.ascii	"LV_USE_ARABIC_PERSIAN_CHARS 0\000"
.LASF139:
	.ascii	"__INTPTR_WIDTH__ 32\000"
.LASF310:
	.ascii	"__ACCUM_FBIT__ 15\000"
.LASF316:
	.ascii	"__UACCUM_IBIT__ 16\000"
.LASF253:
	.ascii	"__FLT32X_MIN__ 1.1\000"
.LASF132:
	.ascii	"__INT_FAST64_MAX__ 0x7fffffffffffffffLL\000"
.LASF250:
	.ascii	"__FLT32X_DECIMAL_DIG__ 17\000"
.LASF506:
	.ascii	"LV_STDLIB_RTTHREAD 3\000"
.LASF59:
	.ascii	"__INT_FAST8_TYPE__ int\000"
.LASF372:
	.ascii	"__UDA_FBIT__ 32\000"
.LASF233:
	.ascii	"__FLT64_MAX_10_EXP__ 308\000"
.LASF93:
	.ascii	"__UINTMAX_C(c) c ## ULL\000"
.LASF31:
	.ascii	"__SIZEOF_POINTER__ 4\000"
.LASF51:
	.ascii	"__INT_LEAST8_TYPE__ signed char\000"
.LASF381:
	.ascii	"__GCC_ATOMIC_BOOL_LOCK_FREE 1\000"
.LASF395:
	.ascii	"__GCC_HAVE_DWARF2_CFI_ASM 1\000"
.LASF615:
	.ascii	"LV_FONT_MONTSERRAT_18 0\000"
.LASF447:
	.ascii	"__ARM_NEON__\000"
.LASF693:
	.ascii	"LV_USE_TILEVIEW 1\000"
.LASF216:
	.ascii	"__FLT32_MAX_EXP__ 128\000"
.LASF631:
	.ascii	"LV_FONT_MONTSERRAT_28_COMPRESSED 0\000"
.LASF450:
	.ascii	"__THUMB_INTERWORK__ 1\000"
.LASF298:
	.ascii	"__ULLFRACT_MAX__ 0XFFFFFFFFFFFFFFFFP-64ULLR\000"
.LASF522:
	.ascii	"LV_STDBOOL_INCLUDE <stdbool.h>\000"
.LASF636:
	.ascii	"LV_FONT_UNSCII_16 0\000"
.LASF561:
	.ascii	"LV_USE_DRAW_SW_COMPLEX_GRADIENTS 0\000"
.LASF33:
	.ascii	"__GNUC_WIDE_EXECUTION_CHARSET_NAME \"UTF-32LE\"\000"
.LASF756:
	.ascii	"LV_USE_WAYLAND 0\000"
.LASF687:
	.ascii	"LV_USE_SPINNER 1\000"
.LASF41:
	.ascii	"__CHAR32_TYPE__ long unsigned int\000"
.LASF519:
	.ascii	"LV_USE_STDLIB_SPRINTF LV_STDLIB_BUILTIN\000"
.LASF444:
	.ascii	"__ARM_FEATURE_FP16_VECTOR_ARITHMETIC\000"
.LASF514:
	.ascii	"LV_CONF_KCONFIG_H \000"
.LASF178:
	.ascii	"__DBL_IS_IEC_60559__ 1\000"
.LASF736:
	.ascii	"LV_USE_SVG_ANIMATION 0\000"
.LASF136:
	.ascii	"__UINT_FAST32_MAX__ 0xffffffffU\000"
.LASF652:
	.ascii	"LV_USE_ARC 1\000"
.LASF151:
	.ascii	"__FLT_MAX_EXP__ 128\000"
.LASF17:
	.ascii	"__SIZEOF_LONG__ 4\000"
.LASF592:
	.ascii	"LV_USE_OBJ_ID_BUILTIN 1\000"
.LASF567:
	.ascii	"LV_USE_DRAW_VG_LITE 0\000"
.LASF21:
	.ascii	"__SIZEOF_DOUBLE__ 8\000"
.LASF417:
	.ascii	"__ARM_FEATURE_CMSE\000"
.LASF778:
	.ascii	"LV_USE_OPENGLES 0\000"
.LASF501:
	.ascii	"LV_OS_SDL2 7\000"
.LASF669:
	.ascii	"LV_USE_IMAGEBUTTON 1\000"
.LASF560:
	.ascii	"LV_USE_DRAW_SW_ASM LV_DRAW_SW_ASM_NONE\000"
.LASF529:
	.ascii	"LV_MEM_POOL_INCLUDE\000"
.LASF728:
	.ascii	"LV_USE_RLOTTIE 0\000"
.LASF509:
	.ascii	"LV_DRAW_SW_ASM_NEON 1\000"
.LASF659:
	.ascii	"LV_CALENDAR_DEFAULT_DAY_NAMES {\"Su\", \"Mo\", \"Tu"
	.ascii	"\", \"We\", \"Th\", \"Fr\", \"Sa\"}\000"
.LASF547:
	.ascii	"LV_DRAW_SW_SUPPORT_ARGB8888 1\000"
.LASF153:
	.ascii	"__FLT_DECIMAL_DIG__ 9\000"
.LASF653:
	.ascii	"LV_USE_ARCLABEL 1\000"
.LASF50:
	.ascii	"__UINT64_TYPE__ long long unsigned int\000"
.LASF801:
	.ascii	"LV_USE_VG_LITE_THORVG 0\000"
.LASF190:
	.ascii	"__LDBL_EPSILON__ 1.1\000"
.LASF378:
	.ascii	"__GNUC_STDC_INLINE__ 1\000"
.LASF442:
	.ascii	"__ARM_FP16_ARGS 1\000"
.LASF690:
	.ascii	"LV_USE_TABVIEW 1\000"
.LASF334:
	.ascii	"__LLACCUM_EPSILON__ 0x1P-31LLK\000"
.LASF7:
	.ascii	"__GNUC_PATCHLEVEL__ 1\000"
.LASF120:
	.ascii	"__UINT_LEAST16_MAX__ 0xffff\000"
.LASF735:
	.ascii	"LV_USE_SVG 0\000"
.LASF594:
	.ascii	"LV_USE_OBJ_PROPERTY_NAME 1\000"
.LASF551:
	.ascii	"LV_DRAW_SW_SUPPORT_A8 1\000"
.LASF804:
	.ascii	"LV_LOG_TRACE_TIMER 0\000"
.LASF524:
	.ascii	"LV_LIMITS_INCLUDE <limits.h>\000"
.LASF620:
	.ascii	"LV_FONT_MONTSERRAT_28 0\000"
.LASF148:
	.ascii	"__FLT_DIG__ 6\000"
.LASF388:
	.ascii	"__GCC_ATOMIC_LONG_LOCK_FREE 1\000"
.LASF65:
	.ascii	"__UINT_FAST32_TYPE__ unsigned int\000"
.LASF3:
	.ascii	"__STDC_UTF_32__ 1\000"
.LASF301:
	.ascii	"__SACCUM_IBIT__ 8\000"
.LASF152:
	.ascii	"__FLT_MAX_10_EXP__ 38\000"
.LASF205:
	.ascii	"__FLT16_MIN__ 1.1\000"
.LASF711:
	.ascii	"LV_USE_FS_ARDUINO_ESP_LITTLEFS 0\000"
.LASF273:
	.ascii	"__FRACT_MAX__ 0X7FFFP-15R\000"
.LASF630:
	.ascii	"LV_FONT_MONTSERRAT_48 0\000"
.LASF530:
	.ascii	"LV_MEM_POOL_ALLOC\000"
.LASF504:
	.ascii	"LV_STDLIB_CLIB 1\000"
.LASF130:
	.ascii	"__INT_FAST32_MAX__ 0x7fffffff\000"
.LASF207:
	.ascii	"__FLT16_DENORM_MIN__ 1.1\000"
.LASF455:
	.ascii	"__ARM_ARCH_EXT_IDIV__\000"
.LASF571:
	.ascii	"LV_USE_DRAW_EVE 0\000"
.LASF103:
	.ascii	"__UINT16_MAX__ 0xffff\000"
.LASF348:
	.ascii	"__TQ_FBIT__ 127\000"
.LASF71:
	.ascii	"__SHRT_MAX__ 0x7fff\000"
.LASF604:
	.ascii	"LV_ATTRIBUTE_FAST_MEM \000"
.LASF174:
	.ascii	"__DBL_DENORM_MIN__ ((double)1.1)\000"
.LASF638:
	.ascii	"LV_FONT_DEFAULT &lv_font_montserrat_14\000"
.LASF523:
	.ascii	"LV_INTTYPES_INCLUDE <inttypes.h>\000"
.LASF211:
	.ascii	"__FLT16_IS_IEC_60559__ 1\000"
.LASF19:
	.ascii	"__SIZEOF_SHORT__ 2\000"
.LASF335:
	.ascii	"__ULLACCUM_FBIT__ 32\000"
.LASF796:
	.ascii	"LV_USE_DEMO_HIGH_RES 0\000"
.LASF600:
	.ascii	"LV_ATTRIBUTE_MEM_ALIGN_SIZE 1\000"
.LASF22:
	.ascii	"__SIZEOF_LONG_DOUBLE__ 8\000"
.LASF396:
	.ascii	"__PRAGMA_REDEFINE_EXTNAME 1\000"
.LASF36:
	.ascii	"__WCHAR_TYPE__ unsigned int\000"
.LASF371:
	.ascii	"__USA_IBIT__ 16\000"
.LASF596:
	.ascii	"LV_BIG_ENDIAN_SYSTEM 0\000"
.LASF263:
	.ascii	"__SFRACT_MAX__ 0X7FP-7HR\000"
.LASF589:
	.ascii	"LV_USE_OBJ_ID 0\000"
.LASF66:
	.ascii	"__UINT_FAST64_TYPE__ long long unsigned int\000"
.LASF805:
	.ascii	"LV_LOG_TRACE_INDEV 0\000"
.LASF16:
	.ascii	"__SIZEOF_INT__ 4\000"
.LASF360:
	.ascii	"__HA_FBIT__ 7\000"
.LASF496:
	.ascii	"LV_OS_FREERTOS 2\000"
.LASF454:
	.ascii	"__FDPIC__\000"
.LASF227:
	.ascii	"__FLT32_IS_IEC_60559__ 1\000"
.LASF786:
	.ascii	"LV_USE_DEMO_RENDER 0\000"
.LASF255:
	.ascii	"__FLT32X_DENORM_MIN__ 1.1\000"
.LASF201:
	.ascii	"__FLT16_MAX_10_EXP__ 4\000"
.LASF750:
	.ascii	"LV_USE_TEST 0\000"
.LASF266:
	.ascii	"__USFRACT_IBIT__ 0\000"
.LASF113:
	.ascii	"__INT32_C(c) c ## L\000"
.LASF564:
	.ascii	"LV_USE_G2D 0\000"
.LASF267:
	.ascii	"__USFRACT_MIN__ 0.0UHR\000"
.LASF448:
	.ascii	"__ARM_NEON\000"
.LASF102:
	.ascii	"__UINT8_MAX__ 0xff\000"
.LASF593:
	.ascii	"LV_USE_OBJ_PROPERTY 0\000"
.LASF183:
	.ascii	"__LDBL_MAX_EXP__ 1024\000"
.LASF686:
	.ascii	"LV_USE_SPINBOX 1\000"
.LASF175:
	.ascii	"__DBL_HAS_DENORM__ 1\000"
.LASF598:
	.ascii	"LV_ATTRIBUTE_TIMER_HANDLER \000"
.LASF568:
	.ascii	"LV_USE_DRAW_DMA2D 0\000"
.LASF364:
	.ascii	"__DA_FBIT__ 31\000"
.LASF109:
	.ascii	"__INT_LEAST16_MAX__ 0x7fff\000"
.LASF720:
	.ascii	"LV_USE_GIF 0\000"
.LASF339:
	.ascii	"__ULLACCUM_EPSILON__ 0x1P-32ULLK\000"
.LASF614:
	.ascii	"LV_FONT_MONTSERRAT_16 0\000"
.LASF500:
	.ascii	"LV_OS_MQX 6\000"
.LASF535:
	.ascii	"LV_DRAW_BUF_ALIGN 4\000"
.LASF123:
	.ascii	"__UINT32_C(c) c ## UL\000"
.LASF755:
	.ascii	"LV_USE_X11 0\000"
.LASF317:
	.ascii	"__UACCUM_MIN__ 0.0UK\000"
.LASF737:
	.ascii	"LV_USE_SVG_DEBUG 0\000"
.LASF502:
	.ascii	"LV_OS_CUSTOM 255\000"
.LASF641:
	.ascii	"LV_USE_FONT_PLACEHOLDER 1\000"
.LASF806:
	.ascii	"LV_LOG_TRACE_DISP_REFR 0\000"
.LASF202:
	.ascii	"__FLT16_DECIMAL_DIG__ 5\000"
.LASF624:
	.ascii	"LV_FONT_MONTSERRAT_36 0\000"
.LASF632:
	.ascii	"LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0\000"
.LASF434:
	.ascii	"__ARM_ARCH_ISA_THUMB\000"
.LASF508:
	.ascii	"LV_DRAW_SW_ASM_NONE 0\000"
.LASF461:
	.ascii	"__ARM_FEATURE_MATMUL_INT8\000"
.LASF562:
	.ascii	"LV_USE_NEMA_GFX 0\000"
.LASF386:
	.ascii	"__GCC_ATOMIC_SHORT_LOCK_FREE 1\000"
.LASF117:
	.ascii	"__INT_LEAST64_WIDTH__ 64\000"
.LASF305:
	.ascii	"__USACCUM_FBIT__ 8\000"
.LASF390:
	.ascii	"__GCC_ATOMIC_TEST_AND_SET_TRUEVAL 1\000"
.LASF8:
	.ascii	"__VERSION__ \"14.3.1 20250623\"\000"
.LASF730:
	.ascii	"LV_USE_VECTOR_GRAPHIC 0\000"
.LASF320:
	.ascii	"__LACCUM_FBIT__ 31\000"
.LASF819:
	.ascii	"LV_USE_THORVG 0\000"
.LASF226:
	.ascii	"__FLT32_HAS_QUIET_NAN__ 1\000"
.LASF759:
	.ascii	"LV_USE_LINUX_DRM 0\000"
.LASF782:
	.ascii	"LV_BUILD_DEMOS 1\000"
.LASF193:
	.ascii	"__LDBL_HAS_INFINITY__ 1\000"
.LASF366:
	.ascii	"__TA_FBIT__ 63\000"
.LASF187:
	.ascii	"__LDBL_MAX__ 1.1\000"
.LASF820:
	.ascii	"LV_USE_EGL LV_LINUX_DRM_USE_EGL\000"
.LASF249:
	.ascii	"__FLT32X_MAX_10_EXP__ 308\000"
.LASF452:
	.ascii	"__ARM_PCS 1\000"
.LASF605:
	.ascii	"LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_"
	.ascii	"warning\000"
.LASF816:
	.ascii	"LV_SYSMON_PROC_IDLE_AVAILABLE 0\000"
.LASF96:
	.ascii	"__SIG_ATOMIC_MIN__ (-__SIG_ATOMIC_MAX__ - 1)\000"
.LASF732:
	.ascii	"LV_USE_THORVG_EXTERNAL 0\000"
.LASF118:
	.ascii	"__UINT_LEAST8_MAX__ 0xff\000"
.LASF692:
	.ascii	"LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500\000"
.LASF478:
	.ascii	"__THUMB 1\000"
.LASF799:
	.ascii	"LV_KCONFIG_PRESENT\000"
.LASF683:
	.ascii	"LV_USE_SLIDER 1\000"
.LASF541:
	.ascii	"LV_USE_DRAW_SW 1\000"
.LASF678:
	.ascii	"LV_USE_LOTTIE 0\000"
.LASF111:
	.ascii	"__INT_LEAST16_WIDTH__ 16\000"
.LASF633:
	.ascii	"LV_FONT_SOURCE_HAN_SANS_SC_14_CJK 0\000"
.LASF610:
	.ascii	"LV_FONT_MONTSERRAT_8 0\000"
.LASF145:
	.ascii	"__DEC_EVAL_METHOD__ 2\000"
.LASF445:
	.ascii	"__ARM_FEATURE_FP16_FML\000"
.LASF173:
	.ascii	"__DBL_EPSILON__ ((double)1.1)\000"
.LASF688:
	.ascii	"LV_USE_SWITCH 1\000"
.LASF269:
	.ascii	"__USFRACT_EPSILON__ 0x1P-8UHR\000"
.LASF265:
	.ascii	"__USFRACT_FBIT__ 8\000"
.LASF333:
	.ascii	"__LLACCUM_MAX__ 0X7FFFFFFFFFFFFFFFP-31LLK\000"
.LASF270:
	.ascii	"__FRACT_FBIT__ 15\000"
.LASF377:
	.ascii	"__USER_LABEL_PREFIX__ \000"
.LASF105:
	.ascii	"__UINT64_MAX__ 0xffffffffffffffffULL\000"
.LASF602:
	.ascii	"LV_ATTRIBUTE_LARGE_CONST \000"
.LASF741:
	.ascii	"LV_USE_PROFILER 0\000"
.LASF1:
	.ascii	"__STDC_VERSION__ 201112L\000"
.LASF106:
	.ascii	"__INT_LEAST8_MAX__ 0x7f\000"
.LASF456:
	.ascii	"__ARM_FEATURE_IDIV\000"
.LASF11:
	.ascii	"__ATOMIC_ACQUIRE 2\000"
.LASF410:
	.ascii	"__ARM_FEATURE_COMPLEX\000"
.LASF574:
	.ascii	"LV_USE_ASSERT_MALLOC 1\000"
.LASF791:
	.ascii	"LV_USE_DEMO_FLEX_LAYOUT 0\000"
.LASF423:
	.ascii	"__ARM_SIZEOF_WCHAR_T 4\000"
.LASF248:
	.ascii	"__FLT32X_MAX_EXP__ 1024\000"
.LASF122:
	.ascii	"__UINT_LEAST32_MAX__ 0xffffffffUL\000"
.LASF672:
	.ascii	"LV_LABEL_TEXT_SELECTION 1\000"
.LASF546:
	.ascii	"LV_DRAW_SW_SUPPORT_XRGB8888 1\000"
.LASF160:
	.ascii	"__FLT_HAS_INFINITY__ 1\000"
.LASF313:
	.ascii	"__ACCUM_MAX__ 0X7FFFFFFFP-15K\000"
.LASF98:
	.ascii	"__INT8_MAX__ 0x7f\000"
.LASF516:
	.ascii	"LV_COLOR_DEPTH 16\000"
.LASF408:
	.ascii	"__ARM_FEATURE_CRC32\000"
.LASF262:
	.ascii	"__SFRACT_MIN__ (-0.5HR-0.5HR)\000"
.LASF363:
	.ascii	"__SA_IBIT__ 16\000"
.LASF406:
	.ascii	"__ARM_FEATURE_UNALIGNED\000"
.LASF618:
	.ascii	"LV_FONT_MONTSERRAT_24 1\000"
.LASF142:
	.ascii	"__GCC_IEC_559_COMPLEX 0\000"
.LASF416:
	.ascii	"__ARM_FEATURE_MVE\000"
.LASF48:
	.ascii	"__UINT16_TYPE__ short unsigned int\000"
.LASF776:
	.ascii	"LV_USE_WINDOWS 0\000"
.LASF601:
	.ascii	"LV_ATTRIBUTE_MEM_ALIGN \000"
.LASF188:
	.ascii	"__LDBL_NORM_MAX__ 1.1\000"
.LASF104:
	.ascii	"__UINT32_MAX__ 0xffffffffUL\000"
.LASF587:
	.ascii	"LV_COLOR_MIX_ROUND_OFS 0\000"
.LASF144:
	.ascii	"__FLT_EVAL_METHOD_TS_18661_3__ 0\000"
.LASF628:
	.ascii	"LV_FONT_MONTSERRAT_44 0\000"
.LASF664:
	.ascii	"LV_USE_CANVAS 1\000"
.LASF665:
	.ascii	"LV_USE_CHART 1\000"
.LASF505:
	.ascii	"LV_STDLIB_MICROPYTHON 2\000"
.LASF63:
	.ascii	"__UINT_FAST8_TYPE__ unsigned int\000"
.LASF331:
	.ascii	"__LLACCUM_IBIT__ 32\000"
.LASF474:
	.ascii	"ARM_MATH_CM0PLUS 1\000"
.LASF689:
	.ascii	"LV_USE_TABLE 1\000"
.LASF358:
	.ascii	"__UTQ_FBIT__ 128\000"
.LASF100:
	.ascii	"__INT32_MAX__ 0x7fffffffL\000"
.LASF115:
	.ascii	"__INT_LEAST64_MAX__ 0x7fffffffffffffffLL\000"
.LASF463:
	.ascii	"__ARM_FEATURE_BF16_VECTOR_ARITHMETIC\000"
.LASF771:
	.ascii	"LV_USE_GENERIC_MIPI 1\000"
.LASF481:
	.ascii	"PICO_DEFAULT_SPI1_DNC_PIN 8\000"
.LASF223:
	.ascii	"__FLT32_DENORM_MIN__ 1.1\000"
.LASF570:
	.ascii	"LV_USE_PPA 0\000"
.LASF212:
	.ascii	"__FLT32_MANT_DIG__ 24\000"
.LASF775:
	.ascii	"LV_USE_NXP_ELCDIF 0\000"
.LASF726:
	.ascii	"LV_USE_FREETYPE 0\000"
.LASF125:
	.ascii	"__UINT64_C(c) c ## ULL\000"
.LASF58:
	.ascii	"__UINT_LEAST64_TYPE__ long long unsigned int\000"
.LASF753:
	.ascii	"LV_USE_COLOR_FILTER 0\000"
.LASF800:
	.ascii	"LV_USE_VG_LITE_DRIVER 0\000"
.LASF383:
	.ascii	"__GCC_ATOMIC_CHAR16_T_LOCK_FREE 1\000"
.LASF743:
	.ascii	"LV_USE_GRIDNAV 0\000"
.LASF774:
	.ascii	"LV_USE_ST_LTDC 0\000"
.LASF167:
	.ascii	"__DBL_MAX_EXP__ 1024\000"
.LASF12:
	.ascii	"__ATOMIC_RELEASE 3\000"
.LASF681:
	.ascii	"LV_USE_ROLLER 1\000"
.LASF256:
	.ascii	"__FLT32X_HAS_DENORM__ 1\000"
.LASF147:
	.ascii	"__FLT_MANT_DIG__ 24\000"
.LASF797:
	.ascii	"LV_USE_DEMO_SMARTWATCH 0\000"
.LASF357:
	.ascii	"__UDQ_IBIT__ 0\000"
.LASF222:
	.ascii	"__FLT32_EPSILON__ 1.1\000"
.LASF660:
	.ascii	"LV_CALENDAR_DEFAULT_MONTH_NAMES {\"January\", \"Feb"
	.ascii	"ruary\", \"March\", \"April\", \"May\", \"June\", \""
	.ascii	"July\", \"August\", \"September\", \"October\", \"N"
	.ascii	"ovember\", \"December\"}\000"
.LASF654:
	.ascii	"LV_USE_BAR 1\000"
.LASF658:
	.ascii	"LV_CALENDAR_WEEK_STARTS_MONDAY 0\000"
.LASF733:
	.ascii	"LV_USE_LZ4_INTERNAL 0\000"
.LASF573:
	.ascii	"LV_USE_ASSERT_NULL 1\000"
.LASF101:
	.ascii	"__INT64_MAX__ 0x7fffffffffffffffLL\000"
.LASF199:
	.ascii	"__FLT16_MIN_10_EXP__ (-4)\000"
.LASF634:
	.ascii	"LV_FONT_SOURCE_HAN_SANS_SC_16_CJK 0\000"
.LASF296:
	.ascii	"__ULLFRACT_IBIT__ 0\000"
.LASF451:
	.ascii	"__ARM_ARCH_6M__ 1\000"
.LASF784:
	.ascii	"LV_USE_DEMO_KEYPAD_AND_ENCODER 0\000"
.LASF239:
	.ascii	"__FLT64_DENORM_MIN__ 1.1\000"
.LASF5:
	.ascii	"__GNUC__ 14\000"
.LASF84:
	.ascii	"__LONG_WIDTH__ 32\000"
.LASF554:
	.ascii	"LV_DRAW_SW_DRAW_UNIT_CNT 1\000"
.LASF315:
	.ascii	"__UACCUM_FBIT__ 16\000"
.LASF675:
	.ascii	"LV_USE_LED 1\000"
.LASF279:
	.ascii	"__UFRACT_EPSILON__ 0x1P-16UR\000"
.LASF76:
	.ascii	"__WCHAR_MIN__ 0U\000"
.LASF191:
	.ascii	"__LDBL_DENORM_MIN__ 1.1\000"
.LASF351:
	.ascii	"__UQQ_IBIT__ 0\000"
.LASF488:
	.ascii	"RAM_PRESET_VALUE 0xDEADBEEF\000"
.LASF746:
	.ascii	"LV_USE_OBSERVER 1\000"
.LASF259:
	.ascii	"__FLT32X_IS_IEC_60559__ 1\000"
.LASF327:
	.ascii	"__ULACCUM_MIN__ 0.0ULK\000"
.LASF428:
	.ascii	"__ARM_ARCH 6\000"
.LASF818:
	.ascii	"LV_USE_LZ4 0\000"
.LASF146:
	.ascii	"__FLT_RADIX__ 2\000"
.LASF821:
	.ascii	"LV_FS_IS_VALID_LETTER(l) ((l) == '/' || ((l) >= 'A'"
	.ascii	" && (l) <= 'Z'))\000"
.LASF515:
	.ascii	"LV_CONF_H \000"
.LASF518:
	.ascii	"LV_USE_STDLIB_STRING LV_STDLIB_BUILTIN\000"
.LASF704:
	.ascii	"LV_FS_DEFAULT_DRIVER_LETTER '\\0'\000"
.LASF738:
	.ascii	"LV_USE_FFMPEG 0\000"
.LASF85:
	.ascii	"__LONG_LONG_WIDTH__ 64\000"
.LASF566:
	.ascii	"LV_USE_DRAW_SDL 0\000"
.LASF563:
	.ascii	"LV_USE_PXP 0\000"
.LASF137:
	.ascii	"__UINT_FAST64_MAX__ 0xffffffffffffffffULL\000"
.LASF439:
	.ascii	"__ARM_FP\000"
.LASF361:
	.ascii	"__HA_IBIT__ 8\000"
.LASF400:
	.ascii	"__ARM_FEATURE_DSP\000"
.LASF619:
	.ascii	"LV_FONT_MONTSERRAT_26 0\000"
.LASF389:
	.ascii	"__GCC_ATOMIC_LLONG_LOCK_FREE 1\000"
.LASF822:
	.ascii	"GNU C11 14.3.1 20250623 -fmessage-length=0 -std=gnu"
	.ascii	"11 -mcpu=cortex-m0plus -mlittle-endian -mfloat-abi="
	.ascii	"soft -mabi=aapcs -mthumb -mno-unaligned-access -mtp"
	.ascii	"=soft -mfp16-format=ieee -gdwarf-4 -g3 -gpubnames -"
	.ascii	"fomit-frame-pointer -ffunction-sections -fdata-sect"
	.ascii	"ions -fshort-enums -fno-common\000"
.LASF232:
	.ascii	"__FLT64_MAX_EXP__ 1024\000"
.LASF556:
	.ascii	"LV_USE_NATIVE_HELIUM_ASM 0\000"
.LASF170:
	.ascii	"__DBL_MAX__ ((double)1.1)\000"
.LASF622:
	.ascii	"LV_FONT_MONTSERRAT_32 0\000"
.LASF795:
	.ascii	"LV_USE_DEMO_EBIKE 0\000"
.LASF543:
	.ascii	"LV_DRAW_SW_SUPPORT_RGB565_SWAPPED 1\000"
.LASF749:
	.ascii	"LV_USE_FONT_MANAGER 0\000"
.LASF537:
	.ascii	"LV_DRAW_LAYER_SIMPLE_BUF_SIZE (24 * 1024)\000"
.LASF46:
	.ascii	"__INT64_TYPE__ long long int\000"
.LASF189:
	.ascii	"__LDBL_MIN__ 1.1\000"
.LASF24:
	.ascii	"__CHAR_BIT__ 8\000"
.LASF397:
	.ascii	"__SIZEOF_WCHAR_T__ 4\000"
.LASF761:
	.ascii	"LV_USE_LOVYAN_GFX 0\000"
.LASF252:
	.ascii	"__FLT32X_NORM_MAX__ 1.1\000"
.LASF528:
	.ascii	"LV_MEM_ADR 0\000"
.LASF694:
	.ascii	"LV_USE_WIN 1\000"
.LASF278:
	.ascii	"__UFRACT_MAX__ 0XFFFFP-16UR\000"
.LASF162:
	.ascii	"__FLT_IS_IEC_60559__ 1\000"
.LASF200:
	.ascii	"__FLT16_MAX_EXP__ 16\000"
.LASF595:
	.ascii	"LV_USE_GESTURE_RECOGNITION 0\000"
.LASF412:
	.ascii	"__ARM_FEATURE_PAUTH\000"
.LASF261:
	.ascii	"__SFRACT_IBIT__ 0\000"
.LASF29:
	.ascii	"__BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__\000"
.LASF449:
	.ascii	"__ARM_NEON_FP\000"
.LASF649:
	.ascii	"LV_TXT_COLOR_CMD \"#\"\000"
.LASF676:
	.ascii	"LV_USE_LINE 1\000"
.LASF650:
	.ascii	"LV_WIDGETS_HAS_DEFAULT_VALUE 1\000"
.LASF783:
	.ascii	"LV_USE_DEMO_WIDGETS 0\000"
.LASF411:
	.ascii	"__ARM_32BIT_STATE\000"
.LASF275:
	.ascii	"__UFRACT_FBIT__ 16\000"
.LASF356:
	.ascii	"__UDQ_FBIT__ 64\000"
.LASF706:
	.ascii	"LV_USE_FS_POSIX 0\000"
.LASF184:
	.ascii	"__LDBL_MAX_10_EXP__ 308\000"
.LASF203:
	.ascii	"__FLT16_MAX__ 1.1\000"
.LASF661:
	.ascii	"LV_USE_CALENDAR_HEADER_ARROW 1\000"
.LASF61:
	.ascii	"__INT_FAST32_TYPE__ int\000"
.LASF680:
	.ascii	"LV_USE_MSGBOX 1\000"
.LASF251:
	.ascii	"__FLT32X_MAX__ 1.1\000"
.LASF10:
	.ascii	"__ATOMIC_SEQ_CST 5\000"
.LASF430:
	.ascii	"__GCC_ASM_FLAG_OUTPUTS__\000"
.LASF707:
	.ascii	"LV_USE_FS_WIN32 0\000"
.LASF149:
	.ascii	"__FLT_MIN_EXP__ (-125)\000"
.LASF242:
	.ascii	"__FLT64_HAS_QUIET_NAN__ 1\000"
.LASF769:
	.ascii	"LV_USE_NV3007 0\000"
.LASF306:
	.ascii	"__USACCUM_IBIT__ 8\000"
.LASF520:
	.ascii	"LV_STDINT_INCLUDE <stdint.h>\000"
.LASF240:
	.ascii	"__FLT64_HAS_DENORM__ 1\000"
.LASF336:
	.ascii	"__ULLACCUM_IBIT__ 32\000"
.LASF319:
	.ascii	"__UACCUM_EPSILON__ 0x1P-16UK\000"
.LASF540:
	.ascii	"LV_DRAW_THREAD_PRIO LV_THREAD_PRIO_HIGH\000"
.LASF548:
	.ascii	"LV_DRAW_SW_SUPPORT_ARGB8888_PREMULTIPLIED 1\000"
.LASF457:
	.ascii	"__ARM_ASM_SYNTAX_UNIFIED__\000"
.LASF143:
	.ascii	"__FLT_EVAL_METHOD__ 0\000"
.LASF580:
	.ascii	"LV_USE_REFR_DEBUG 0\000"
.LASF70:
	.ascii	"__SCHAR_MAX__ 0x7f\000"
.LASF127:
	.ascii	"__INT_FAST8_WIDTH__ 32\000"
.LASF2:
	.ascii	"__STDC_UTF_16__ 1\000"
.LASF432:
	.ascii	"__thumb2__\000"
.LASF555:
	.ascii	"LV_USE_DRAW_ARM2D_SYNC 0\000"
.LASF785:
	.ascii	"LV_USE_DEMO_BENCHMARK 0\000"
.LASF418:
	.ascii	"__ARM_FEATURE_LDREX\000"
.LASF651:
	.ascii	"LV_USE_ANIMIMG 1\000"
.LASF538:
	.ascii	"LV_DRAW_LAYER_MAX_MEMORY 0\000"
.LASF37:
	.ascii	"__WINT_TYPE__ unsigned int\000"
.LASF141:
	.ascii	"__GCC_IEC_559 0\000"
.LASF671:
	.ascii	"LV_USE_LABEL 1\000"
.LASF581:
	.ascii	"LV_USE_LAYER_DEBUG 0\000"
.LASF283:
	.ascii	"__LFRACT_MAX__ 0X7FFFFFFFP-31LR\000"
.LASF399:
	.ascii	"__SIZEOF_PTRDIFF_T__ 4\000"
.LASF0:
	.ascii	"__STDC__ 1\000"
.LASF597:
	.ascii	"LV_ATTRIBUTE_TICK_INC \000"
.LASF493:
	.ascii	"LV_CONF_INTERNAL_H \000"
.LASF437:
	.ascii	"__SOFTFP__ 1\000"
.LASF30:
	.ascii	"__FLOAT_WORD_ORDER__ __ORDER_LITTLE_ENDIAN__\000"
.LASF716:
	.ascii	"LV_USE_LIBPNG 0\000"
.LASF172:
	.ascii	"__DBL_MIN__ ((double)1.1)\000"
.LASF482:
	.ascii	"PICO_DEFAULT_SPI1_CSN_PIN 9\000"
.LASF824:
	.ascii	"E:\\GIT-Repos\\RaspiPico\\DualCore\\Core1\000"
.LASF55:
	.ascii	"__UINT_LEAST8_TYPE__ unsigned char\000"
.LASF682:
	.ascii	"LV_USE_SCALE 1\000"
.LASF747:
	.ascii	"LV_USE_IME_PINYIN 0\000"
.LASF82:
	.ascii	"__SHRT_WIDTH__ 16\000"
.LASF494:
	.ascii	"LV_OS_NONE 0\000"
.LASF757:
	.ascii	"LV_USE_LINUX_FBDEV 0\000"
.LASF370:
	.ascii	"__USA_FBIT__ 16\000"
.LASF182:
	.ascii	"__LDBL_MIN_10_EXP__ (-307)\000"
.LASF644:
	.ascii	"LV_TXT_LINE_BREAK_LONG_LEN 0\000"
.LASF616:
	.ascii	"LV_FONT_MONTSERRAT_20 0\000"
.LASF558:
	.ascii	"LV_DRAW_SW_SHADOW_CACHE_SIZE 0\000"
.LASF533:
	.ascii	"LV_USE_OS LV_OS_NONE\000"
.LASF691:
	.ascii	"LV_USE_TEXTAREA 1\000"
.LASF284:
	.ascii	"__LFRACT_EPSILON__ 0x1P-31LR\000"
.LASF171:
	.ascii	"__DBL_NORM_MAX__ ((double)1.1)\000"
.LASF422:
	.ascii	"__ARM_SIZEOF_MINIMAL_ENUM 1\000"
.LASF426:
	.ascii	"__arm__ 1\000"
.LASF626:
	.ascii	"LV_FONT_MONTSERRAT_40 1\000"
.LASF274:
	.ascii	"__FRACT_EPSILON__ 0x1P-15R\000"
.LASF215:
	.ascii	"__FLT32_MIN_10_EXP__ (-37)\000"
.LASF772:
	.ascii	"LV_COLOR_16_SWAP 0\000"
.LASF685:
	.ascii	"LV_SPAN_SNIPPET_STACK_SIZE 64\000"
.LASF441:
	.ascii	"__ARM_FP16_FORMAT_ALTERNATIVE\000"
.LASF810:
	.ascii	"LV_LOG_TRACE_ANIM 0\000"
.LASF25:
	.ascii	"__BIGGEST_ALIGNMENT__ 8\000"
.LASF814:
	.ascii	"LV_USE_PERF_MONITOR 0\000"
.LASF367:
	.ascii	"__TA_IBIT__ 64\000"
.LASF585:
	.ascii	"LV_IMAGE_HEADER_CACHE_DEF_CNT 0\000"
.LASF472:
	.ascii	"RP2040_M0_1 1\000"
.LASF673:
	.ascii	"LV_LABEL_LONG_TXT_HINT 1\000"
.LASF760:
	.ascii	"LV_USE_TFT_ESPI 0\000"
.LASF407:
	.ascii	"__ARM_FEATURE_QRDMX\000"
.LASF435:
	.ascii	"__ARM_ARCH_ISA_THUMB 1\000"
.LASF781:
	.ascii	"LV_BUILD_EXAMPLES 0\000"
.LASF74:
	.ascii	"__LONG_LONG_MAX__ 0x7fffffffffffffffLL\000"
.LASF490:
	.ascii	"LIB_PICO_PRINTF_PICO 1\000"
.LASF480:
	.ascii	"DISP_SPI_NUM 1\000"
.LASF87:
	.ascii	"__WINT_WIDTH__ 32\000"
.LASF725:
	.ascii	"LV_USE_BARCODE 0\000"
.LASF591:
	.ascii	"LV_OBJ_ID_AUTO_ASSIGN LV_USE_OBJ_ID\000"
.LASF276:
	.ascii	"__UFRACT_IBIT__ 0\000"
.LASF446:
	.ascii	"__ARM_FEATURE_FMA\000"
.LASF107:
	.ascii	"__INT8_C(c) c\000"
.LASF281:
	.ascii	"__LFRACT_IBIT__ 0\000"
.LASF517:
	.ascii	"LV_USE_STDLIB_MALLOC LV_STDLIB_BUILTIN\000"
.LASF393:
	.ascii	"__GCC_ATOMIC_POINTER_LOCK_FREE 1\000"
.LASF165:
	.ascii	"__DBL_MIN_EXP__ (-1021)\000"
.LASF192:
	.ascii	"__LDBL_HAS_DENORM__ 1\000"
.LASF325:
	.ascii	"__ULACCUM_FBIT__ 32\000"
.LASF384:
	.ascii	"__GCC_ATOMIC_CHAR32_T_LOCK_FREE 1\000"
.LASF590:
	.ascii	"LV_USE_OBJ_NAME 1\000"
.LASF213:
	.ascii	"__FLT32_DIG__ 6\000"
.LASF684:
	.ascii	"LV_USE_SPAN 1\000"
.LASF532:
	.ascii	"LV_DPI_DEF 130\000"
.LASF719:
	.ascii	"LV_USE_LIBJPEG_TURBO 0\000"
.LASF507:
	.ascii	"LV_STDLIB_CUSTOM 255\000"
.LASF492:
	.ascii	"LV_DRAW_DMA2D_H \000"
.LASF394:
	.ascii	"__HAVE_SPECULATION_SAFE_VALUE 1\000"
.LASF300:
	.ascii	"__SACCUM_FBIT__ 7\000"
.LASF788:
	.ascii	"LV_USE_DEMO_MUSIC 0\000"
.LASF802:
	.ascii	"LV_LOG_LEVEL LV_LOG_LEVEL_NONE\000"
.LASF579:
	.ascii	"LV_ASSERT_HANDLER while(1);\000"
.LASF312:
	.ascii	"__ACCUM_MIN__ (-0X1P15K-0X1P15K)\000"
.LASF403:
	.ascii	"__ARM_FEATURE_CRYPTO\000"
.LASF53:
	.ascii	"__INT_LEAST32_TYPE__ long int\000"
.LASF709:
	.ascii	"LV_USE_FS_MEMFS 0\000"
.LASF789:
	.ascii	"LV_USE_DEMO_VECTOR_GRAPHIC 0\000"
.LASF544:
	.ascii	"LV_DRAW_SW_SUPPORT_RGB565A8 1\000"
.LASF424:
	.ascii	"__ARM_ARCH_PROFILE\000"
.LASF124:
	.ascii	"__UINT_LEAST64_MAX__ 0xffffffffffffffffULL\000"
.LASF271:
	.ascii	"__FRACT_IBIT__ 0\000"
.LASF243:
	.ascii	"__FLT64_IS_IEC_60559__ 1\000"
.LASF26:
	.ascii	"__ORDER_LITTLE_ENDIAN__ 1234\000"
.LASF637:
	.ascii	"LV_FONT_CUSTOM_DECLARE LV_FONT_DECLARE(w_font_bahn_"
	.ascii	"sbc_24) LV_FONT_DECLARE(w_font_bahn_sbc_40) LV_FONT"
	.ascii	"_DECLARE(w_font_bahn_sbc_60) LV_FONT_DECLARE(w_font"
	.ascii	"_bahn_sbc_80) LV_FONT_DECLARE(w_font_bahn_sbc_100) "
	.ascii	"LV_FONT_DECLARE(w_font_bahn_sbc_120) LV_FONT_DECLAR"
	.ascii	"E(rb_font_adlam_24) LV_FONT_DECLARE(rb_font_adlam_4"
	.ascii	"0) LV_FONT_DECLARE(rb_font_adlam_60) LV_FONT_DECLAR"
	.ascii	"E(rb_font_adlam_80) LV_FONT_DECLARE(rb_font_adlam_1"
	.ascii	"00) LV_FONT_DECLARE(rb_font_adlam_120) LV_FONT_DECL"
	.ascii	"ARE(w_font_stencil_24) LV_FONT_DECLARE(w_font_stenc"
	.ascii	"il_40) LV_FONT_DECLARE(w_font_stencil_60) LV_FONT_D"
	.ascii	"ECLARE(w_font_stencil_80) LV_FONT_DECLARE(w_font_st"
	.ascii	"encil_100) LV_FONT_DECLARE(w_font_stencil_120)\000"
.LASF114:
	.ascii	"__INT_LEAST32_WIDTH__ 32\000"
.LASF534:
	.ascii	"LV_DRAW_BUF_STRIDE_ALIGN 1\000"
.LASF150:
	.ascii	"__FLT_MIN_10_EXP__ (-37)\000"
.LASF326:
	.ascii	"__ULACCUM_IBIT__ 32\000"
.LASF195:
	.ascii	"__LDBL_IS_IEC_60559__ 1\000"
.LASF670:
	.ascii	"LV_USE_KEYBOARD 1\000"
.LASF35:
	.ascii	"__PTRDIFF_TYPE__ int\000"
.LASF703:
	.ascii	"LV_USE_GRID 1\000"
.LASF429:
	.ascii	"__APCS_32__ 1\000"
.LASF346:
	.ascii	"__DQ_FBIT__ 63\000"
.LASF47:
	.ascii	"__UINT8_TYPE__ unsigned char\000"
.LASF812:
	.ascii	"LV_WAYLAND_WINDOW_DECORATIONS 0\000"
.LASF487:
	.ascii	"PICO_TIME_DEFAULT_ALARM_POOL_HARDWARE_ALARM_NUM 2\000"
.LASF353:
	.ascii	"__UHQ_IBIT__ 0\000"
.LASF557:
	.ascii	"LV_DRAW_SW_COMPLEX 1\000"
.LASF60:
	.ascii	"__INT_FAST16_TYPE__ int\000"
.LASF302:
	.ascii	"__SACCUM_MIN__ (-0X1P7HK-0X1P7HK)\000"
.LASF288:
	.ascii	"__ULFRACT_MAX__ 0XFFFFFFFFP-32ULR\000"
.LASF56:
	.ascii	"__UINT_LEAST16_TYPE__ short unsigned int\000"
.LASF157:
	.ascii	"__FLT_EPSILON__ 1.1\000"
.LASF323:
	.ascii	"__LACCUM_MAX__ 0X7FFFFFFFFFFFFFFFP-31LK\000"
.LASF40:
	.ascii	"__CHAR16_TYPE__ short unsigned int\000"
.LASF525:
	.ascii	"LV_STDARG_INCLUDE <stdarg.h>\000"
.LASF245:
	.ascii	"__FLT32X_DIG__ 15\000"
.LASF156:
	.ascii	"__FLT_MIN__ 1.1\000"
.LASF584:
	.ascii	"LV_CACHE_DEF_SIZE 0\000"
.LASF204:
	.ascii	"__FLT16_NORM_MAX__ 1.1\000"
.LASF15:
	.ascii	"__FINITE_MATH_ONLY__ 0\000"
.LASF128:
	.ascii	"__INT_FAST16_MAX__ 0x7fffffff\000"
.LASF727:
	.ascii	"LV_USE_TINY_TTF 0\000"
.LASF807:
	.ascii	"LV_LOG_TRACE_EVENT 0\000"
.LASF328:
	.ascii	"__ULACCUM_MAX__ 0XFFFFFFFFFFFFFFFFP-32ULK\000"
.LASF625:
	.ascii	"LV_FONT_MONTSERRAT_38 0\000"
.LASF599:
	.ascii	"LV_ATTRIBUTE_FLUSH_READY \000"
.LASF287:
	.ascii	"__ULFRACT_MIN__ 0.0ULR\000"
.LASF347:
	.ascii	"__DQ_IBIT__ 0\000"
.LASF464:
	.ascii	"__ARM_BF16_FORMAT_ALTERNATIVE\000"
.LASF45:
	.ascii	"__INT32_TYPE__ long int\000"
.LASF612:
	.ascii	"LV_FONT_MONTSERRAT_12 0\000"
.LASF510:
	.ascii	"LV_DRAW_SW_ASM_HELIUM 2\000"
.LASF798:
	.ascii	"LV_USE_ANIMIMAGE LV_USE_ANIMIMG\000"
.LASF536:
	.ascii	"LV_DRAW_TRANSFORM_USE_MATRIX 0\000"
.LASF792:
	.ascii	"LV_USE_DEMO_MULTILANG 0\000"
.LASF359:
	.ascii	"__UTQ_IBIT__ 0\000"
.LASF362:
	.ascii	"__SA_FBIT__ 15\000"
.LASF640:
	.ascii	"LV_USE_FONT_COMPRESSED 0\000"
.LASF453:
	.ascii	"__ARM_EABI__ 1\000"
.LASF466:
	.ascii	"__ELF__ 1\000"
.LASF433:
	.ascii	"__THUMBEL__ 1\000"
.LASF793:
	.ascii	"LV_USE_DEMO_TRANSFORM 0\000"
.LASF341:
	.ascii	"__QQ_IBIT__ 0\000"
.LASF608:
	.ascii	"LV_USE_MATRIX 0\000"
.LASF639:
	.ascii	"LV_FONT_FMT_TXT_LARGE 0\000"
.LASF572:
	.ascii	"LV_USE_LOG 0\000"
.LASF221:
	.ascii	"__FLT32_MIN__ 1.1\000"
.LASF39:
	.ascii	"__UINTMAX_TYPE__ long long unsigned int\000"
.LASF699:
	.ascii	"LV_THEME_DEFAULT_TRANSITION_TIME 80\000"
.LASF355:
	.ascii	"__USQ_IBIT__ 0\000"
.LASF6:
	.ascii	"__GNUC_MINOR__ 3\000"
.LASF57:
	.ascii	"__UINT_LEAST32_TYPE__ long unsigned int\000"
.LASF420:
	.ascii	"__ARM_FEATURE_NUMERIC_MAXMIN\000"
.LASF38:
	.ascii	"__INTMAX_TYPE__ long long int\000"
.LASF387:
	.ascii	"__GCC_ATOMIC_INT_LOCK_FREE 1\000"
.LASF476:
	.ascii	"BOOT_STAGE2_WINBOND_W25Q080 1\000"
.LASF549:
	.ascii	"LV_DRAW_SW_SUPPORT_L8 1\000"
.LASF318:
	.ascii	"__UACCUM_MAX__ 0XFFFFFFFFP-16UK\000"
.LASF698:
	.ascii	"LV_THEME_DEFAULT_GROW 1\000"
.LASF811:
	.ascii	"LV_WAYLAND_USE_DMABUF 0\000"
.LASF443:
	.ascii	"__ARM_FEATURE_FP16_SCALAR_ARITHMETIC\000"
.LASF309:
	.ascii	"__USACCUM_EPSILON__ 0x1P-8UHK\000"
.LASF177:
	.ascii	"__DBL_HAS_QUIET_NAN__ 1\000"
.LASF577:
	.ascii	"LV_USE_ASSERT_OBJ 0\000"
.LASF293:
	.ascii	"__LLFRACT_MAX__ 0X7FFFFFFFFFFFFFFFP-63LLR\000"
.LASF404:
	.ascii	"__ARM_FEATURE_AES\000"
.LASF768:
	.ascii	"LV_USE_FT81X 0\000"
.LASF303:
	.ascii	"__SACCUM_MAX__ 0X7FFFP-7HK\000"
.LASF67:
	.ascii	"__INTPTR_TYPE__ int\000"
.LASF236:
	.ascii	"__FLT64_NORM_MAX__ 1.1\000"
.LASF483:
	.ascii	"PICO_DEFAULT_SPI1_SCK_PIN 10\000"
.LASF667:
	.ascii	"LV_USE_DROPDOWN 1\000"
.LASF376:
	.ascii	"__REGISTER_PREFIX__ \000"
.LASF164:
	.ascii	"__DBL_DIG__ 15\000"
.LASF289:
	.ascii	"__ULFRACT_EPSILON__ 0x1P-32ULR\000"
.LASF415:
	.ascii	"__ARM_FEATURE_PAC_DEFAULT\000"
.LASF155:
	.ascii	"__FLT_NORM_MAX__ 1.1\000"
.LASF23:
	.ascii	"__SIZEOF_SIZE_T__ 4\000"
.LASF575:
	.ascii	"LV_USE_ASSERT_STYLE 1\000"
.LASF116:
	.ascii	"__INT64_C(c) c ## LL\000"
.LASF498:
	.ascii	"LV_OS_RTTHREAD 4\000"
.LASF459:
	.ascii	"__ARM_FEATURE_CDE\000"
.LASF311:
	.ascii	"__ACCUM_IBIT__ 16\000"
.LASF479:
	.ascii	"DO_RAM_PRESET 1\000"
.LASF762:
	.ascii	"LV_USE_EVDEV 0\000"
.LASF491:
	.ascii	"LV_DRAW_DMA2D_PRIVATE_H \000"
.LASF121:
	.ascii	"__UINT16_C(c) c\000"
.LASF230:
	.ascii	"__FLT64_MIN_EXP__ (-1021)\000"
.LASF373:
	.ascii	"__UDA_IBIT__ 32\000"
.LASF379:
	.ascii	"__NO_INLINE__ 1\000"
.LASF715:
	.ascii	"LV_USE_LODEPNG 0\000"
.LASF803:
	.ascii	"LV_LOG_TRACE_MEM 0\000"
.LASF503:
	.ascii	"LV_STDLIB_BUILTIN 0\000"
.LASF752:
	.ascii	"LV_USE_TRANSLATION 0\000"
.LASF9:
	.ascii	"__ATOMIC_RELAXED 0\000"
.LASF655:
	.ascii	"LV_USE_BUTTON 1\000"
.LASF458:
	.ascii	"__ARM_FEATURE_COPROC\000"
.LASF629:
	.ascii	"LV_FONT_MONTSERRAT_46 0\000"
.LASF176:
	.ascii	"__DBL_HAS_INFINITY__ 1\000"
.LASF95:
	.ascii	"__SIG_ATOMIC_MAX__ 0x7fffffff\000"
.LASF744:
	.ascii	"LV_USE_FRAGMENT 0\000"
.LASF228:
	.ascii	"__FLT64_MANT_DIG__ 53\000"
.LASF198:
	.ascii	"__FLT16_MIN_EXP__ (-13)\000"
.LASF20:
	.ascii	"__SIZEOF_FLOAT__ 4\000"
.LASF169:
	.ascii	"__DBL_DECIMAL_DIG__ 17\000"
.LASF697:
	.ascii	"LV_THEME_DEFAULT_DARK 0\000"
.LASF531:
	.ascii	"LV_DEF_REFR_PERIOD 33\000"
.LASF647:
	.ascii	"LV_USE_BIDI 0\000"
.LASF382:
	.ascii	"__GCC_ATOMIC_CHAR_LOCK_FREE 1\000"
.LASF99:
	.ascii	"__INT16_MAX__ 0x7fff\000"
.LASF552:
	.ascii	"LV_DRAW_SW_SUPPORT_I1 1\000"
.LASF578:
	.ascii	"LV_ASSERT_HANDLER_INCLUDE <stdint.h>\000"
.LASF745:
	.ascii	"LV_USE_IMGFONT 0\000"
.LASF83:
	.ascii	"__INT_WIDTH__ 32\000"
.LASF219:
	.ascii	"__FLT32_MAX__ 1.1\000"
.LASF623:
	.ascii	"LV_FONT_MONTSERRAT_34 0\000"
.LASF340:
	.ascii	"__QQ_FBIT__ 7\000"
.LASF553:
	.ascii	"LV_DRAW_SW_I1_LUM_THRESHOLD 127\000"
.LASF674:
	.ascii	"LV_LABEL_WAIT_CHAR_COUNT 3\000"
.LASF97:
	.ascii	"__SIG_ATOMIC_WIDTH__ 32\000"
.LASF375:
	.ascii	"__UTA_IBIT__ 64\000"
.LASF724:
	.ascii	"LV_USE_QRCODE 0\000"
.LASF280:
	.ascii	"__LFRACT_FBIT__ 31\000"
.LASF299:
	.ascii	"__ULLFRACT_EPSILON__ 0x1P-64ULLR\000"
.LASF54:
	.ascii	"__INT_LEAST64_TYPE__ long long int\000"
.LASF398:
	.ascii	"__SIZEOF_WINT_T__ 4\000"
.LASF112:
	.ascii	"__INT_LEAST32_MAX__ 0x7fffffffL\000"
.LASF465:
	.ascii	"__GXX_TYPEINFO_EQUALITY_INLINE 0\000"
.LASF186:
	.ascii	"__LDBL_DECIMAL_DIG__ 17\000"
.LASF588:
	.ascii	"LV_OBJ_STYLE_CACHE 0\000"
.LASF668:
	.ascii	"LV_USE_IMAGE 1\000"
.LASF322:
	.ascii	"__LACCUM_MIN__ (-0X1P31LK-0X1P31LK)\000"
.LASF126:
	.ascii	"__INT_FAST8_MAX__ 0x7fffffff\000"
.LASF224:
	.ascii	"__FLT32_HAS_DENORM__ 1\000"
.LASF539:
	.ascii	"LV_DRAW_THREAD_STACK_SIZE (8 * 1024)\000"
.LASF337:
	.ascii	"__ULLACCUM_MIN__ 0.0ULLK\000"
.LASF131:
	.ascii	"__INT_FAST32_WIDTH__ 32\000"
.LASF385:
	.ascii	"__GCC_ATOMIC_WCHAR_T_LOCK_FREE 1\000"
.LASF471:
	.ascii	"__CROSSWORKS_REVISION 0\000"
.LASF49:
	.ascii	"__UINT32_TYPE__ long unsigned int\000"
.LASF409:
	.ascii	"__ARM_FEATURE_DOTPROD\000"
.LASF813:
	.ascii	"LV_LINUX_DRM_USE_EGL 0\000"
.LASF701:
	.ascii	"LV_USE_THEME_MONO 1\000"
.LASF559:
	.ascii	"LV_DRAW_SW_CIRCLE_CACHE_SIZE 4\000"
.LASF512:
	.ascii	"LV_NEMA_HAL_CUSTOM 0\000"
.LASF440:
	.ascii	"__ARM_FP16_FORMAT_IEEE 1\000"
.LASF297:
	.ascii	"__ULLFRACT_MIN__ 0.0ULLR\000"
.LASF787:
	.ascii	"LV_USE_DEMO_STRESS 0\000"
.LASF809:
	.ascii	"LV_LOG_TRACE_LAYOUT 0\000"
.LASF475:
	.ascii	"FLASH_PLACEMENT 1\000"
.LASF392:
	.ascii	"__GCC_CONSTRUCTIVE_SIZE 64\000"
.LASF635:
	.ascii	"LV_FONT_UNSCII_8 0\000"
.LASF291:
	.ascii	"__LLFRACT_IBIT__ 0\000"
.LASF751:
	.ascii	"LV_USE_XML 0\000"
.LASF197:
	.ascii	"__FLT16_DIG__ 3\000"
.LASF550:
	.ascii	"LV_DRAW_SW_SUPPORT_AL88 1\000"
.LASF613:
	.ascii	"LV_FONT_MONTSERRAT_14 1\000"
.LASF64:
	.ascii	"__UINT_FAST16_TYPE__ unsigned int\000"
.LASF780:
	.ascii	"LV_USE_QNX 0\000"
.LASF369:
	.ascii	"__UHA_IBIT__ 8\000"
.LASF470:
	.ascii	"__CROSSWORKS_MINOR_VERSION 3\000"
.LASF314:
	.ascii	"__ACCUM_EPSILON__ 0x1P-15K\000"
.LASF329:
	.ascii	"__ULACCUM_EPSILON__ 0x1P-32ULK\000"
.LASF582:
	.ascii	"LV_USE_PARALLEL_DRAW_DEBUG 0\000"
.LASF180:
	.ascii	"__LDBL_DIG__ 15\000"
.LASF499:
	.ascii	"LV_OS_WINDOWS 5\000"
.LASF700:
	.ascii	"LV_USE_THEME_SIMPLE 1\000"
.LASF723:
	.ascii	"LV_USE_RLE 0\000"
.LASF89:
	.ascii	"__SIZE_WIDTH__ 32\000"
.LASF606:
	.ascii	"LV_ATTRIBUTE_EXTERN_DATA \000"
.LASF78:
	.ascii	"__WINT_MIN__ 0U\000"
.LASF229:
	.ascii	"__FLT64_DIG__ 15\000"
.LASF77:
	.ascii	"__WINT_MAX__ 0xffffffffU\000"
.LASF721:
	.ascii	"LV_USE_GSTREAMER 0\000"
.LASF108:
	.ascii	"__INT_LEAST8_WIDTH__ 8\000"
.LASF52:
	.ascii	"__INT_LEAST16_TYPE__ short int\000"
.LASF511:
	.ascii	"LV_DRAW_SW_ASM_CUSTOM 255\000"
.LASF645:
	.ascii	"LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3\000"
.LASF290:
	.ascii	"__LLFRACT_FBIT__ 63\000"
.LASF225:
	.ascii	"__FLT32_HAS_INFINITY__ 1\000"
.LASF206:
	.ascii	"__FLT16_EPSILON__ 1.1\000"
.LASF431:
	.ascii	"__thumb__ 1\000"
.LASF436:
	.ascii	"__ARMEL__ 1\000"
.LASF705:
	.ascii	"LV_USE_FS_STDIO 0\000"
.LASF462:
	.ascii	"__ARM_FEATURE_BF16_SCALAR_ARITHMETIC\000"
.LASF342:
	.ascii	"__HQ_FBIT__ 15\000"
.LASF779:
	.ascii	"LV_USE_GLFW 0\000"
.LASF527:
	.ascii	"LV_MEM_POOL_EXPAND_SIZE 0\000"
.LASF714:
	.ascii	"LV_USE_FS_FROGFS 0\000"
.LASF763:
	.ascii	"LV_USE_LIBINPUT 0\000"
.LASF80:
	.ascii	"__SIZE_MAX__ 0xffffffffU\000"
.LASF402:
	.ascii	"__ARM_FEATURE_SAT\000"
.LASF427:
	.ascii	"__ARM_ARCH\000"
.LASF808:
	.ascii	"LV_LOG_TRACE_OBJ_CREATE 0\000"
.LASF220:
	.ascii	"__FLT32_NORM_MAX__ 1.1\000"
.LASF73:
	.ascii	"__LONG_MAX__ 0x7fffffffL\000"
.LASF583:
	.ascii	"LV_ENABLE_GLOBAL_CUSTOM 0\000"
.LASF729:
	.ascii	"LV_USE_GLTF 0\000"
.LASF764:
	.ascii	"LV_USE_ST7735 0\000"
.LASF196:
	.ascii	"__FLT16_MANT_DIG__ 11\000"
.LASF740:
	.ascii	"LV_USE_SYSMON 0\000"
.LASF294:
	.ascii	"__LLFRACT_EPSILON__ 0x1P-63LLR\000"
.LASF421:
	.ascii	"__ARM_FEATURE_SIMD32\000"
.LASF244:
	.ascii	"__FLT32X_MANT_DIG__ 53\000"
.LASF86:
	.ascii	"__WCHAR_WIDTH__ 32\000"
.LASF110:
	.ascii	"__INT16_C(c) c\000"
.LASF365:
	.ascii	"__DA_IBIT__ 32\000"
.LASF210:
	.ascii	"__FLT16_HAS_QUIET_NAN__ 1\000"
.LASF405:
	.ascii	"__ARM_FEATURE_SHA2\000"
.LASF542:
	.ascii	"LV_DRAW_SW_SUPPORT_RGB565 1\000"
.LASF13:
	.ascii	"__ATOMIC_ACQ_REL 4\000"
.LASF817:
	.ascii	"LV_USE_PERF_MONITOR_LOG_MODE 0\000"
.LASF343:
	.ascii	"__HQ_IBIT__ 0\000"
.LASF166:
	.ascii	"__DBL_MIN_10_EXP__ (-307)\000"
.LASF695:
	.ascii	"LV_USE_3DTEXTURE 0\000"
.LASF304:
	.ascii	"__SACCUM_EPSILON__ 0x1P-7HK\000"
.LASF609:
	.ascii	"LV_USE_PRIVATE_API 0\000"
.LASF158:
	.ascii	"__FLT_DENORM_MIN__ 1.1\000"
.LASF646:
	.ascii	"LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3\000"
.LASF92:
	.ascii	"__UINTMAX_MAX__ 0xffffffffffffffffULL\000"
.LASF163:
	.ascii	"__DBL_MANT_DIG__ 53\000"
.LASF696:
	.ascii	"LV_USE_THEME_DEFAULT 1\000"
.LASF286:
	.ascii	"__ULFRACT_IBIT__ 0\000"
.LASF663:
	.ascii	"LV_USE_CALENDAR_CHINESE 0\000"
.LASF330:
	.ascii	"__LLACCUM_FBIT__ 31\000"
.LASF72:
	.ascii	"__INT_MAX__ 0x7fffffff\000"
.LASF603:
	.ascii	"LV_ATTRIBUTE_LARGE_RAM_ARRAY \000"
.LASF713:
	.ascii	"LV_USE_FS_UEFI 0\000"
.LASF460:
	.ascii	"__ARM_FEATURE_CDE_COPROC\000"
	.ident	"GCC: (based on arm-14.3.Rel1 source release) 14.3.1 20250623"
