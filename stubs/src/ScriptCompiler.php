<?php declare(strict_types=1);

/**
 * This file is part of the pinepain/php-v8 PHP extension.
 *
 * Copyright (c) 2015-2017 Bogdan Padalko <pinepain@gmail.com>
 *
 * Licensed under the MIT license: http://opensource.org/licenses/MIT
 *
 * For the full copyright and license information, please view the
 * LICENSE file that was distributed with this source or visit
 * http://opensource.org/licenses/MIT
 */


namespace V8;


use V8\ScriptCompiler\Source;


/**
 * For compiling scripts.
 */
class ScriptCompiler
{
    const OPTION_NO_COMPILE_OPTIONS      = 0;
    const OPTION_PRODUCE_PARSER_CACHE    = 1;
    const OPTION_CONSUME_PARSER_CACHE    = 2;
    const OPTION_PRODUCE_CODE_CACHE      = 3;
    const OPTION_PRODUCE_FULL_CODE_CACHE = 4;
    const OPTION_CONSUME_CODE_CACHE      = 5;

    private function __construct()
    {
    }

    /**
     * Return a version tag for CachedData for the current V8 version & flags.
     *
     * This value is meant only for determining whether a previously generated
     * CachedData instance is still valid; the tag has no other meaing.
     *
     * Background: The data carried by CachedData may depend on the exact
     *   V8 version number or currently compiler flags. This means when
     *   persisting CachedData, the embedder must take care to not pass in
     *   data from another V8 version, or the same version with different
     *   features enabled.
     *
     *   The easiest way to do so is to clear the embedder's cache on any
     *   such change.
     *
     *   Alternatively, this tag can be stored alongside the cached data and
     *   compared when it is being used.
     *
     * @return float
     */
    public static function getCachedDataVersionTag(): float
    {
    }

    /**
     * Compiles the specified script (context-independent).
     * Cached data as part of the source object can be optionally produced to be
     * consumed later to speed up compilation of identical source scripts.
     *
     * Note that when producing cached data, the source must point to NULL for
     * cached data. When consuming cached data, the cached data must have been
     * produced by the same version of V8.
     *
     * \param source Script source code.
     * \return Compiled script object (context independent; for running it must be
     *   bound to a context).
     *
     * @param Context $context
     * @param Source  $source
     * @param int     $options
     *
     * @return UnboundScript
     */
    public static function compileUnboundScript(Context $context, Source $source, int $options = ScriptCompiler::OPTION_NO_COMPILE_OPTIONS): UnboundScript
    {
    }

    /**
     * Compiles the specified script (bound to current context).
     *
     * \param source Script source code.
     * \param pre_data Pre-parsing data, as obtained by ScriptData::PreCompile()
     *   using pre_data speeds compilation if it's done multiple times.
     *   Owned by caller, no references are kept when this function returns.
     * \return Compiled script object, bound to the context that was active
     *   when this function was called. When run it will always use this
     *   context.
     *
     * @param Context $context
     * @param Source  $source
     * @param int     $options
     *
     * @return Script
     */
    public static function compile(Context $context, Source $source, int $options = ScriptCompiler::OPTION_NO_COMPILE_OPTIONS): Script
    {
    }

    /**
     * Compile a function for a given context. This is equivalent to running
     *
     * with (obj) {
     *   return function(args) { ... }
     * }
     *
     * It is possible to specify multiple context extensions (obj in the above
     * example).
     *
     * @param Context       $context
     * @param Source        $source
     * @param StringValue[] $arguments
     * @param ObjectValue[] $context_extensions
     *
     * @return FunctionObject
     */
    public static function compileFunctionInContext(Context $context, Source $source, array $arguments = [], array $context_extensions = []): FunctionObject
    {
    }
}
