#pragma once

namespace KHAS {

	enum class ActionWithInputValue {
		LoopIsError
		, ErrorReturn
	};

	enum class Commands {
		AddingAPair
		, DeletingAPair
		, SetNewSET
		, Exit
		, Unknown
		, None
	};

}