${SegmentFile}

${SegmentPrePrimary}
	ReadINIStr $0 "$EXEDIR\Data\IrfanView\license.ini" "Registration" "Name"
	${If} $0 != ""
		WriteINIStr "$EXEDIR\App\IrfanView\i_view32.ini" "Registration" "Name" $0
		ReadINIStr $0 "$EXEDIR\Data\IrfanView\license.ini" "Registration" "Code"
		WriteINIStr "$EXEDIR\App\IrfanView\i_view32.ini" "Registration" "Code" $0
	${EndIf}
!macroend

${SegmentPostPrimary}
	DeleteINISec "$EXEDIR\App\IrfanView\i_view32.ini" "Registration"
!macroend
