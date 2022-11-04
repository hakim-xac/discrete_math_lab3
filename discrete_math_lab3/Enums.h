#pragma once

namespace KHAS {

	enum class ActionWithInputValue {
		LoopIsError
		, ErrorReturn
	};

	enum class Commands {
		AddingAPair
		, DeletingAPair
		, RemovingAPair
		, SetNewSET
		, Exit
		, Unknown
		, None
	};

}