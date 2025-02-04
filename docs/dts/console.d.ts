interface Console {

  /**
   * Log values to the terminal.
   *
   * @param values Values to log to the terminal.
   */
  log(...values: any[]): void;
}

declare var console: Console;
